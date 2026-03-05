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

void TerrainCustom::init(const int num_cols = 4, const int num_rows = 4,
    const float height_mult = 1.0f, const float size = 1.0f,
    const bool is_centered = true){
    if (is_initialized_) return;
    is_initialized_ = true;

  //TODO demand graphic resources to the GPUManager.
    auto& GPU = *EDK::dev::GPUManager::Instance();
    GPU.newBuffer(&elements_buffer);
    GPU.newBuffer(&order_buffer);

  //****** Positions:
    const int vertices = (num_cols + 1) * (num_rows + 1);
    
    scoped_array<esat::Vec3> positions;
    positions.alloc(vertices);

    float size_x = num_cols * size;
    float size_y = num_rows * size;
    float center_x = size_x / num_cols;
    float center_y = size_y / num_rows;
    float part_x = center_x/



    0 = i*(num_cols+1) * j
    5 = (i+1)*(num_cols+1)+j


    for (int i = 0; i < num_cols + 1; i++) {
        for (int j = 0; j < num_rows + 1; j++) {


            positions[i * (j + num_rows + 1)] = { size_x ,0.0f };
        }
    }
    

  //****** Normals:
    esat::Vec3 normals[4] = {
        Vec3Normalized(position[0]),
        Vec3Normalized(position[1]),
        Vec3Normalized(position[2]),
        Vec3Normalized(position[3])
    };

  //****** UVs:
    esat::Vec2 uvs[4] = {
        {0.0f,1.0f},
        {0.0f,0.0f},
        {1.0f,0.0f},
        {1.0f,1.0f},
    };


    struct Vertex {
        esat::Vec3 pos;
        esat::Vec3 normal;
        esat::Vec2 uv;
    };

    Vertex vertices[4];

    for (int i = 0; i < 4; i++) {
        vertices[i] = { position[i], normals[i], uvs[i] };
    }

  //****** Upload data:
  //TODO initialize and upload data to the "elements_buffer".
  //Size in bytes: 8 data per element * 1 faces * 
  //               4 vertices per face * sizeof(float)
  //36 attributes has a face.
    int size = 8 * 1 * 4 * sizeof(float);
    elements_buffer->init(EDK::dev::Buffer::Target::kTarget_Array_Buffer, size);
    elements_buffer->uploadData(&vertices->pos.x, size, 0);

  //****** Upload order:
  //TODO initialize and upload data to the "order_buffer".
    

    int elements_order[6] = { 0,1,2, 0,2,3 };
    order_buffer->init(EDK::dev::Buffer::Target::kTarget_Element_Array_Buffer, sizeof(elements_order));
    order_buffer->uploadData(elements_order, sizeof(elements_order), 0);
   
  //Remember to order triangles in counter clockwise direction!!
  //6 vertices per face * 1 faces * sizeof(short int)
}

const bool TerrainCustom::bindAttribute(const Attribute a,
                         unsigned int where_to_bind_attribute) const {

    typedef EDK::dev::GPUManager GPU;

    switch (a) {
        case Attribute::A_POSITION:
            GPU::Instance()->enableVertexAttribute(elements_buffer.get(), where_to_bind_attribute, EDK::Type::T_FLOAT_3, false,0 * sizeof(float), 8 * sizeof(float));
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
    GPU::Instance()->drawElements(GPU::DrawMode::kDrawMode_Triangles,6,
                                  order_buffer.get(), EDK::Type::T_UINT,0);
}

} //EDK
