/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Render target class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK3_RENDER_TARGET__
#define __EDK3_RENDER_TARGET__

#include "edk_ref_ptr.h"
#include "edk_scoped_ptr.h"
#include "edk_texture.h"


namespace EDK {

class RenderTarget : public Referenced {
 public:
  RenderTarget();
  void init(const unsigned int width, const unsigned int height, 
            const unsigned int color_attachments = 1);
  const Texture* color_texture(unsigned int index = 0) const;
  const Texture* depth_texture() const;
  Texture* color_texture(unsigned int index = 0);
  Texture* depth_texture();
  //From now on, every draw-call will end up in the RenderTarget
  void begin() const;
  //Finishes the rendering process inside the rendertarget
  void end() const;

  unsigned int width() const;
  unsigned int height() const;
  static RenderTarget* CurrentRenderTarget();
  struct Data;

 protected:
  virtual ~RenderTarget();

 private:
  RenderTarget(const RenderTarget&);
  RenderTarget& operator=(const RenderTarget &);
  scoped_ptr<Data> data_;
}; //RenderTarget

} //EDK

#endif //__EDK3_RENDER_TARGET__
