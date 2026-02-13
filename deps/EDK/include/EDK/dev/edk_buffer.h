/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Graphic buffer class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_DEV_BUFFER_H__
#define __EDK_DEV_BUFFER_H__

#include "EDK/edk_referenced.h"
#include "EDK/edk_scoped_array.h"


namespace EDK {

namespace dev {

/* OpenGL calls:
 *        glBindBuffer
 *        glBufferData
 *        glBufferSubData
 *        glDeleteBuffers
 *        glGenBuffers
 */

class Buffer : public Referenced {
 public:
  enum Target {
    kTarget_Array_Buffer,         //Vertices info.
    kTarget_Element_Array_Buffer, //Vertices order.
  };
  virtual void init(const Buffer::Target t, const unsigned int size) = 0;
  virtual void bind(const Target t) const = 0;
  virtual void uploadData(const void *data, 
                          unsigned int size, 
                          unsigned int offset = 0) = 0;
  virtual void release() = 0;
  virtual const unsigned int size() const = 0;

 protected:
  Buffer() {}
  virtual ~Buffer() {}

 private:
  Buffer(const Buffer&);
  Buffer& operator=(const Buffer&);
}; //Buffer

} //dev

} //EDK

#endif //__EDK_DEV_BUFFER_H__
