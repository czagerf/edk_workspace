/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Graphic program class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho, <isancho@esat.es>
 *
 */


#ifndef __EDK_DEV_PROGRAM_H__
#define __EDK_DEV_PROGRAM_H__

#include "EDK/edk_referenced.h"
#include "EDK/edk_scoped_array.h"
#include "EDK/edk_constants.h"

typedef unsigned int GLuint;


namespace EDK {

namespace dev {

class Shader;
class Uniform;

/* OpenGL calls:
 *        glAttachShader
 *        glDeleteProgram
 *        glLinkProgram
 *        glUseProgram
 *        glGetUniformLocation
 *        glCreateProgram  
 *        glGetProgramiv
 *        glGetProgramInfoLog
 *        glGetAttribLocation
 *        glUniform(1f,2f,Matrix4f,...)
 *        glBindAttribLocation // optional
 */

class Program : public virtual Referenced {
 public:
  virtual void attach(Shader *) = 0;
  virtual const bool link(scoped_array<char> *link_log = 0L) = 0; 
  virtual void use() const = 0; 

  virtual const int get_attrib_location(const char *name) const = 0;
  virtual const int get_uniform_position(const char *name) const = 0;

  virtual void set_uniform_value(const int uniform_pos,
                                 const Type uniform_type,
                                 const float *) const = 0;
  virtual void set_uniform_value(const int uniform_pos,
                                 const Type uniform_type,
                                 const int*) const = 0;
  virtual void set_uniform_value(const int uniform_pos,
                                 const Type uniform_type,
                                 const unsigned int*) const = 0;

  /** returns the internal program ID*/
  virtual const GLuint internal_id() const = 0;

 protected:
  Program() {}
  virtual ~Program() {}

 private:
  Program(const Program&);
  Program& operator=(const Program&);
}; //Program

} //dev

} //EDK

#endif //__EDK_DEV_PROGRAM_H__
