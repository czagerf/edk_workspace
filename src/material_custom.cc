/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom material source file.
 *
 */


#include "material_custom.h"
#include "EDK/dev/edk_shader.h"
#include "EDK/dev/edk_gpu_manager.h"
#include <fstream>

namespace EDK {

    static char kExampleVertexShader1[] =
        "#version 330\n"
        "uniform...";

#define GLSL(x) "#version 330\n"#x
    static char* kVertexShader; 
    /*
    = GLSL(
    uniform mat4 u_vp_matrix;
    uniform mat4 u_m_matrix;

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec3 a_normal;
    layout(location = 2) in vec3 a_uv;

    void main() {
        gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
    }
    );
    */

    static const char* kFragmentShader;
    /*
    = GLSL(
        uniform vec4 u_color;

    out vec4 FinalColor;

    void main() {
        //FinalColor = vec4(1.0, 0.0, 0.0, 1.0);
        FinalColor = u_color;
    }
        );
    */

    //IMPORTANT!!!
    //Type the constructors, destructor and operator= here, just in this point!!!
    //If you don't do it, it won't compile!!!

    MaterialCustom::MaterialCustom() {}
    MaterialCustom::~MaterialCustom() {}
    MaterialCustom::MaterialCustom(const MaterialCustom& other) { program_ = other.program_; }
    MaterialCustom& MaterialCustom::operator=(const MaterialCustom& other) {
        program_ = other.program_;
        return *this;
    }

    void readFile(const char* path, scoped_array<char>& buffer) {
      std::ifstream file_descriptor;

      file_descriptor.open(path);

      if (file_descriptor.is_open()) {
        file_descriptor.seekg(0, std::ios::end);
        int size = file_descriptor.tellg();
        file_descriptor.seekg(0, std::ios::beg);

        char* ptr = buffer.alloc(size);
        buffer.memClear();
       
        file_descriptor.read(ptr, size);
        file_descriptor.close();
      }
    }
    
    void MaterialCustom::init(const char* vertex_path, const char* fragment_path) {
        EDK::ref_ptr<EDK::dev::Shader> v_shader;
        EDK::ref_ptr<EDK::dev::Shader> f_shader;

        scoped_array<char> vertex_buffer;
        scoped_array<char> fragment_buffer;
        
        readFile(vertex_path, vertex_buffer);
        readFile(fragment_path, fragment_buffer);

        kVertexShader = vertex_buffer.get();
        kFragmentShader = fragment_buffer.get();

        EDK::dev::GPUManager::Instance()->newShader(&v_shader);
        EDK::dev::GPUManager::Instance()->newShader(&f_shader);
        EDK::dev::GPUManager::Instance()->newProgram(&program_);

        v_shader->loadSource(EDK::dev::Shader::Type::kType_Vertex, kVertexShader, strlen(kVertexShader));
        f_shader->loadSource(EDK::dev::Shader::Type::kType_Fragment, kFragmentShader, strlen(kFragmentShader));

        vertex_buffer.memClear();
        fragment_buffer.memClear();
        vertex_buffer.release();
        fragment_buffer.release();

        EDK::scoped_array<char> log;
        if (false == v_shader->compile(&log)) {
            printf("[Vertex Error]: %s\n", log.get());
        }
        if (false == f_shader->compile(&log)) {
            printf("[Vertex Error]: %s\n", log.get());
        }

        program_->attach(v_shader.get());
        program_->attach(f_shader.get());

        if (program_->link(&log)) {
            printf("Program Error: % s\n", log.get());
        }

        //1: Request at least two shaders and one program to the GPU Manager.
        //2: Load the source code to the requested shaders.
        //3: Compile both shaders.
        //4: Attach shaders to the program.
        //5: Finally... link the program!
    }

    const bool MaterialCustom::enable(const EDK::MaterialSettings* mat) const {
        //Enable the material...
        //... and use the uniforms located in the material settings!
        program_->use();
        typedef MaterialCustomSettings MBS;
        const MBS* ms = dynamic_cast<const MBS*>(mat);
        if (ms) {
            int loc = program_->get_uniform_position("u_color");
            program_->set_uniform_value(loc, EDK::Type::T_FLOAT_4, ms->color());
            return true;
        }

        return false;
    }

    void MaterialCustom::setupCamera(const float projection[16],
        const float view[16]) const {
        //Set the projection and view matrices as uniforms here! 
        esat::Mat4 v = esat::Mat4FromColumns(view);
        esat::Mat4 p = esat::Mat4FromColumns(projection);
        esat::Mat4 vp = esat::Mat4Multiply(p, v);
        int loc = program_->get_uniform_position("u_vp_matrix");
        program_->set_uniform_value(loc, EDK::Type::T_MAT_4x4, vp.d);
    }

    void MaterialCustom::setupModel(const float model[16]) const {
        //Set the model matrix as uniform here!
        int loc = program_->get_uniform_position("u_m_matrix");
        program_->set_uniform_value(loc, EDK::Type::T_MAT_4x4, model);
    }

    unsigned int MaterialCustom::num_attributes_required() const {
        //Depending on how attributes the geometry has.
        return 3;  //Position. normal, uv
    }

    EDK::Attribute MaterialCustom::attribute_at_index(const unsigned int attrib_idx) const {
        //Search on "EDK::Attribute" enum.
        //Depending on how attributes are stored in the geometry buffer.
        switch (attrib_idx) {
        case 0: return EDK::Attribute::A_POSITION;  break;
        case 1: return EDK::Attribute::A_NORMAL;  break;
        case 2: return EDK::Attribute::A_UV;  break;
        }
        return EDK::Attribute::A_POSITION;
    }

    EDK::Type MaterialCustom::attribute_type_at_index(const unsigned int attrib_idx) const {
        //Search on "EDK::Type" enum.
        //Depending on how attributes are stored in the geometry buffer.
        switch (attrib_idx) {
        case 0: return EDK::Type::T_FLOAT_3;  break;
        case 1: return EDK::Type::T_FLOAT_3;  break;
        case 2: return EDK::Type::T_FLOAT_2;  break;
        }
        return EDK::Type::T_FLOAT_3;

    } //EDK
}