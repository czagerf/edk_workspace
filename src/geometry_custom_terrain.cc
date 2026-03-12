/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom quad source file.
 *
 */

#include "geometry_custom_terrain.h"
#include "ESAT/math.h"
#include "EDK/dev/edk_gpu_manager.h"


namespace EDK {

TerrainCustom::TerrainCustom() : is_initialized_(false) {}
TerrainCustom::~TerrainCustom() {
    elements_buffer.release();
    order_buffer.release();
}

TerrainCustom::TerrainCustom(const TerrainCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
}


TerrainCustom& TerrainCustom::operator=(const TerrainCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
    return *this;
}

void TerrainCustom::init(const int num_cols, const int num_rows,
    const float height_mult, const float size,
    const bool is_centered){
    if (is_initialized_) return;
    is_initialized_ = true;

    num_cols_ = num_cols;
    num_rows_ = num_rows;

    //TODO demand graphic resources to the GPUManager.
    auto& GPU = *EDK::dev::GPUManager::Instance();
    GPU.newBuffer(&elements_buffer);
    GPU.newBuffer(&order_buffer);

  //****** Positions:
    const int num_vertices = (num_cols + 1) * (num_rows + 1);

    struct Vertex {
      esat::Vec3 pos;
      esat::Vec3 normal;
      esat::Vec2 uv;
    };

    scoped_array<Vertex> vertex;
    vertex.alloc(num_vertices);

    //Centrar si es que toca
    float half_size_x = 0.0f;
    float half_size_y = 0.0f;
    if (is_centered) {
      half_size_x = num_cols * size / 2;
      half_size_y = num_rows * size / 2;
    }

    /*
    Dibujo desde la esquina inferior izquierda hasta
    esquina superior derecha. De esta forma coinciden
    los vertices para calcular facil las UV

    Vertices        UVs

    12 13 14 15   0,0.99 ....         1,1
    8 9 10 11     0,0.66
    4 5 6  7      0,0.33  ......
    0 1 2  3      0,0  0.33,0  0.66,0 1,0
    */
   
    for (int i = 0; i < num_rows + 1; i++) {
      for (int j = 0; j < num_cols + 1; j++) {
        int pos = i * (num_cols + 1) + j;

        vertex[pos] = { 
           { size * j - half_size_x ,0.0f, i * size - half_size_y},
           {0.0f, 1.0f, 0.0f},
           {(float)j/ num_cols, (float)i/num_rows}
        };
      }
    }

    int vertex_size = sizeof(Vertex) * num_vertices; 
    elements_buffer->init(EDK::dev::Buffer::Target::kTarget_Array_Buffer, vertex_size);
    elements_buffer->uploadData(vertex.get(), vertex_size, 0);

    scoped_array<int> elements_order;
    elements_order.alloc(num_cols* num_rows *6);

    int aux = 0;
    for (int i = 0; i < num_rows; i++) {
      for (int j = 0; j < num_cols; j++) {
        //Me ubico en cada quad y saco sus esquinas 
        int up_left = i * (num_cols + 1) + j; 
        int up_right = up_left + 1;
        int down_left = (i + 1) * (num_cols + 1) + j;
        int down_right = down_left + 1;

        //Relleno los 6 vertices para los dos triangulos del quad
        elements_order[aux++] = up_left;
        elements_order[aux++] = down_left;
        elements_order[aux++] = down_right;
        elements_order[aux++] = up_left;
        elements_order[aux++] = down_right;
        elements_order[aux++] = up_right;
      }
    }

    int elements_size = num_cols * num_rows * 6 * sizeof(int);

    order_buffer->init(EDK::dev::Buffer::Target::kTarget_Element_Array_Buffer, elements_size);
    order_buffer->uploadData(elements_order.get(), elements_size, 0);
}

const bool TerrainCustom::bindAttribute(const Attribute a,
                         unsigned int where_to_bind_attribute) const {

    typedef EDK::dev::GPUManager GPU;

    switch (a) {
        case Attribute::A_POSITION:
            GPU::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK::Type::T_FLOAT_3, false,0 * sizeof(float), 8*sizeof(float));
            return true;
            break;
        case Attribute::A_NORMAL:
            GPU::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK::Type::T_FLOAT_3, false, 3 * sizeof(float), 8*sizeof(float));
            return true;
            break;
        case Attribute::A_UV:
            GPU::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK::Type::T_FLOAT_2, false, 6 * sizeof(float), 8*sizeof(float));
            return true;
            break;
    }
    return false;
}

void TerrainCustom::render() const {
  
    typedef EDK::dev::GPUManager GPU;
    GPU::Instance()->drawElements(GPU::DrawMode::kDrawMode_Triangles, num_cols_ * num_rows_ * 6,
                                  order_buffer.get(), EDK::Type::T_UINT,0);
}

} //EDK
