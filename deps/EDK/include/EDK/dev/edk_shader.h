/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Graphic shader class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_DEV_SHADER_H__
#define __EDK_DEV_SHADER_H__

#include "EDK/edk_referenced.h"
#include "EDK/edk_scoped_array.h"

typedef unsigned int GLuint;


namespace EDK {

namespace dev {

/* OpenGL calls:
 *        glCompileShader
 *        glDeleteShader
 *        glShaderSource
 *        glGetShaderiv
 *        glGetShaderInfoLog
 *        glCreateShader
 */

class Shader : public Referenced {
 public:
  // Type of the shader
  enum Type {
    kType_Invalid,
    kType_Vertex,
    kType_Fragment,
    kType_Geometry,
  };

  // Loads the source of the Shader
  virtual void loadSource(const Type shader_type,
                          const char *source,
                          const unsigned int source_size) = 0;

  // Compiles the shader. Optionally it can fill the output log of 
  // the compilation
  // Returns true if the shader was successfully compiled
  virtual const bool compile(scoped_array<char> *output_log = NULL) = 0;

  virtual const Type type() const = 0;
  virtual const bool is_compiled() const = 0;
  virtual const GLuint internal_id() const = 0;

 protected:
  Shader() {}
  virtual ~Shader() {}

 private:
  Shader(const Shader&);
  Shader& operator=(const Shader&);
}; //Shader

} //dev

} //EDK

#endif //__EDK_DEV_SHADER_H__
