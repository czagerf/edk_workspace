/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom quad source file.
 *
 */

#include "geometry_custom_cube.h"
#include "ESAT/math.h"
#include "EDK/dev/edk_gpu_manager.h"


namespace EDK {

CubeCustom::CubeCustom() : is_initialized_(false) {}
CubeCustom::~CubeCustom() {
    elements_buffer.release();
    order_buffer.release();
}

CubeCustom::CubeCustom(const CubeCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
}


CubeCustom& CubeCustom::operator=(const CubeCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
    return *this;
}

esat::Vec3 Vec3Normalized2(const esat::Vec3& v) {
    float h = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    assert(h > 0.0);
    h = 1.0f / h;
    esat::Vec3 tmp{ v.x * h,v.y * h,v.z * h };
    return tmp;
}

void CubeCustom::init24v(const float cube_size) {
  if (is_initialized_) return;
  is_initialized_ = true;

  //TODO demand graphic resources to the GPUManager.
  auto& GPU = *EDK::dev::GPUManager::Instance();
  GPU.newBuffer(&elements_buffer);
  GPU.newBuffer(&order_buffer);

  //****** Positions:
  float h = cube_size * 0.5f;
  esat::Vec3 position[24] = {
    // +Z
    {-h,  h,  h}, {-h, -h,  h}, {h, -h,  h }, {h, h, h},
    // -Z
    {h, h, -h}, {h, -h, -h}, { -h, -h, -h }, { -h,  h, -h },
    // +X
    {h, h, h}, {h, -h, h}, {h, -h, -h}, {h, h, -h},
    // -X
    { -h,  h, -h }, { -h, -h, -h }, { -h, -h,  h }, { -h,  h,  h },
    // +Y
    { -h,  h, -h }, { -h,  h,  h }, {h, h, h}, {h, h, -h},
    // -Y
    { -h, -h,  h }, { -h, -h, -h }, {h, -h, -h}, {h, -h, h}
  };

  //****** Normals:
  esat::Vec3 normals[24] = {
    // +Z
    {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
    // -Z
    {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
    // +X
    {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
    // -X
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    // +Y
    {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
    // -Y
    {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}
  };

  //****** UVs:
  esat::Vec2 uvs[24] = {
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
      {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}
  };

  struct Vertex {
    esat::Vec3 pos;
    esat::Vec3 normal;
    esat::Vec2 uv;
  };

  Vertex vertices[24];

  for (int i = 0; i < 24; i++) {
    vertices[i] = { position[i], normals[i], uvs[i] };
  }

  //****** Upload data:
  //TODO initialize and upload data to the "elements_buffer".
  //Size in bytes: 8 data per element * 1 faces * 
  //               4 vertices per face * sizeof(float)
  //36 attributes has a face.
  int size = sizeof(vertices);
  elements_buffer->init(EDK::dev::Buffer::Target::kTarget_Array_Buffer, size);
  elements_buffer->uploadData(&vertices->pos.x, size, 0);

  //****** Upload order:
  //TODO initialize and upload data to the "order_buffer".


  int elements_order[36] = {
    0,1,2, 0,2,3,      // +Z
    4,5,6, 4,6,7,      // -Z
    8,9,10, 8,10,11,   // +X
    12,13,14, 12,14,15,// -X
    16,17,18, 16,18,19,// +Y
    20,21,22, 20,22,23 // -Y
  };
  order_buffer->init(EDK::dev::Buffer::Target::kTarget_Element_Array_Buffer, sizeof(elements_order));
  order_buffer->uploadData(elements_order, sizeof(elements_order), 0);

  //Remember to order triangles in counter clockwise direction!!
  //6 vertices per face * 1 faces * sizeof(short int)
}

void CubeCustom::init8v(const float cube_size) {
    if (is_initialized_) return;
    is_initialized_ = true;

  //TODO demand graphic resources to the GPUManager.
    auto& GPU = *EDK::dev::GPUManager::Instance();
    GPU.newBuffer(&elements_buffer);
    GPU.newBuffer(&order_buffer);

  //****** Positions:
    float half_size = cube_size * 0.5f;
    esat::Vec3 position[8] = {
        {-half_size,  half_size, half_size},
        {-half_size, -half_size, half_size},
        { half_size, -half_size, half_size},
        { half_size,  half_size, half_size},
        { -half_size,  half_size, -half_size},
        {-half_size, -half_size, -half_size},
        { half_size, -half_size, -half_size},
        { half_size,  half_size, -half_size}
    };

  //****** Normals:
    esat::Vec3 normals[8];
    for (int i = 0; i < 8; i++) {
      normals[i] = Vec3Normalized2(position[i]);
    }

  //****** UVs:
    esat::Vec2 uvs[8] = {
        {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f},
        {0.0f,1.0f}, {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}
    };

    struct Vertex {
        esat::Vec3 pos;
        esat::Vec3 normal;
        esat::Vec2 uv;
    };

    Vertex vertices[8];

    for (int i = 0; i < 8; i++) {
        vertices[i] = { position[i], normals[i], uvs[i] };
    }

  //****** Upload data:
  //TODO initialize and upload data to the "elements_buffer".
  //Size in bytes: 8 data per element * 1 faces * 
  //               4 vertices per face * sizeof(float)
  //36 attributes has a face.
    int size = sizeof(vertices);
    elements_buffer->init(EDK::dev::Buffer::Target::kTarget_Array_Buffer, size);
    elements_buffer->uploadData(&vertices->pos.x, size, 0);

  //****** Upload order:
  //TODO initialize and upload data to the "order_buffer".
    
    int elements_order[36] = { 
        0,1,2, 0,2,3,
        3,2,6, 3,6,7,
        7,6,5, 7,5,4,
        4,5,1, 4,1,0,
        4,0,3, 4,3,7,
        1,5,6, 1,6,2
    };
    order_buffer->init(EDK::dev::Buffer::Target::kTarget_Element_Array_Buffer, sizeof(elements_order));
    order_buffer->uploadData(elements_order, sizeof(elements_order), 0);
   
  //Remember to order triangles in counter clockwise direction!!
  //6 vertices per face * 1 faces * sizeof(short int)
}

const bool CubeCustom::bindAttribute(const Attribute a,
      unsigned int where_to_bind_attribute) const{

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

void CubeCustom::render() const {
  
    typedef EDK::dev::GPUManager GPU;
    GPU::Instance()->drawElements(GPU::DrawMode::kDrawMode_Triangles,36,
                                  order_buffer.get(), EDK::Type::T_UINT,0);
}

} //EDK
