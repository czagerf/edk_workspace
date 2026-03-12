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

#include "Perlin/SimplexNoise.h"

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

esat::Vec3 Vec3Cross(const esat::Vec3& a, const esat::Vec3& b) {
    return esat::Vec3{
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x
    };
}

esat::Vec3 Vec3Sum(const esat::Vec3& a, const esat::Vec3& b) {
    return esat::Vec3{
      a.x + b.x,
      a.y + b.y,
      a.z + b.z
    };
}

esat::Vec3 Vec3Normalized3(const esat::Vec3& v) {
    float h = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    assert(h > 0.0);
    h = 1.0f / h;
    esat::Vec3 tmp{ v.x * h,v.y * h,v.z * h };
    return tmp;
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
    float half_size_z = 0.0f;
    if (is_centered) {
      half_size_x = num_cols * size / 2;
      half_size_z = num_rows * size / 2;
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

    scoped_array<int> elements_order;
    elements_order.alloc(num_cols* num_rows *6);

    int aux = 0;

    for (int i = 0; i < num_rows + 1; i++) {
      for (int j = 0; j < num_cols + 1; j++) {
        int quad = i * (num_cols + 1) + j;

        //Me ubico en cada quad y saco sus esquinas 
        int vert_up_left = i * (num_cols + 1) + j;
        int vert_up_right = vert_up_left + 1;
        int vert_down_left = (i + 1) * (num_cols + 1) + j;
        int vert_down_right = vert_down_left + 1;
        
        esat::Vec3 pos = { j * size - half_size_x ,SimplexNoise::noise(i,j)*2, i * size - half_size_z };

        esat::Vec3 vright = Vec3Normalized3({ size, -pos.y, -pos.z });
        esat::Vec3 vtop = Vec3Normalized3({ -pos.x, -pos.y, size });
        esat::Vec3 vleft = Vec3Normalized3({ -size, -pos.y, -pos.z });
        esat::Vec3 vdown = Vec3Normalized3({ -pos.x, -pos.y, size });
        
        esat::Vec3 normal = Vec3Normalized3(Vec3Cross(vright,vtop));
        normal = Vec3Normalized3(Vec3Cross(normal, vleft));
        normal = Vec3Normalized3(Vec3Cross(normal, vdown));    

        vertex[quad] = { 
           pos,
           normal,
           {(float)j/ num_cols, (float)i/num_rows}
        };                

        if (i < num_rows && j < num_cols) {
            //Relleno los 6 vertices para los dos triangulos del quad
            elements_order[aux++] = vert_up_left;
            elements_order[aux++] = vert_down_left;
            elements_order[aux++] = vert_down_right;
            elements_order[aux++] = vert_up_left;
            elements_order[aux++] = vert_down_right;
            elements_order[aux++] = vert_up_right;
        }
      }
    }

    int vertex_size = sizeof(Vertex) * num_vertices; 
    elements_buffer->init(EDK::dev::Buffer::Target::kTarget_Array_Buffer, vertex_size);
    elements_buffer->uploadData(vertex.get(), vertex_size, 0);

    
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
