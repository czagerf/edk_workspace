/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Graphic framebuffer class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_DEV_FRAMEBUFFER_H__
#define __EDK_DEV_FRAMEBUFFER_H__

#include "EDK/edk_referenced.h"
#include "EDK/edk_ref_ptr.h"
#include "EDK/edk_constants.h"


namespace EDK {

class Texture;

namespace dev {

class Framebuffer : public virtual Referenced {
 public:
  static unsigned int const MAX_COLOR_ATTACHMENTS = 16;
  virtual void attachColorTexture(Texture *, const unsigned int pos) = 0;
  virtual void attachDepthTexture(Texture *) = 0;
  virtual const bool bind() const = 0;

 protected:
  Framebuffer() {}
  ~Framebuffer() {}

 private:
  Framebuffer(const Framebuffer &);
  Framebuffer& operator=(const Framebuffer&);
}; //Framebuffer

} //dev

} //EDK

#endif //__EDK_DEV_FRAMEBUFFER_H__
