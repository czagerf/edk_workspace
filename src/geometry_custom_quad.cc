/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom quad source file.
 *
 */

#include "geometry_custom_quad.h"
#include "ESAT/math.h"
#include "EDK/dev/edk_gpu_manager.h"


namespace EDK {

QuadCustom::QuadCustom() : is_initialized_(false) {}
QuadCustom::~QuadCustom() {
    elements_buffer.release();
    order_buffer.release();
}

QuadCustom::QuadCustom(const QuadCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
}


QuadCustom& QuadCustom::operator=(const QuadCustom& other) {
    elements_buffer = other.elements_buffer;
    order_buffer = other.order_buffer;
    is_initialized_ = other.is_initialized_;
    return *this;
}

esat::Vec3 Vec3Normalized(const esat::Vec3& v) {
    float h = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    assert(h > 0.0);
    h = 1.0f / h;
    esat::Vec3 tmp{ v.x * h,v.y * h,v.z * h };
    return tmp;
}

void QuadCustom::init(const float quad_size) {
    if (is_initialized_) return;
    is_initialized_ = true;

  //TODO demand graphic resources to the GPUManager.
    auto& GPU = *EDK::dev::GPUManager::Instance();
    GPU.newBuffer(&elements_buffer);
    GPU.newBuffer(&order_buffer);

  /**
   The quad face is like this:
       0 ------------3
         |\        |
         | \       |
         |  \      |
         |   \     |
         |    \    |
         |     \   |
         |      \  |
         |       \ |
         |        \|
       1 ------------2
    That's the order of its face.
   */

  //****** Positions:
    float half_size = quad_size * 0.5f;
    esat::Vec3 position[4] = {
        {-half_size,  half_size,0.0f},
        {-half_size, -half_size,0.0f},
        { half_size, -half_size,0.0f},
        { half_size,  half_size,0.0f}
    };

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

const bool QuadCustom::bindAttribute(const Attribute a, 
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

void QuadCustom::render() const {
  
    typedef EDK::dev::GPUManager GPU;
    GPU::Instance()->drawElements(GPU::DrawMode::kDrawMode_Triangles,6,
                                  order_buffer.get(), EDK::Type::T_UINT,0);
}

} //EDK
