/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Texture class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_TEXTURE_H__
#define __EDK_TEXTURE_H__

#include "edk_referenced.h"
#include "edk_ref_ptr.h"
#include "edk_constants.h"

typedef unsigned int GLuint;


namespace EDK {

class Texture : public virtual Referenced {
 public:
  enum Type {
    T_Invalid,
    T_1D,
    T_2D,
    T_3D,
    T_CUBEMAP
  };

  //Internal and external format representation.
  //Not all formats can be used for internal texture representation.
  enum Format {
    F_None,
    F_R,
    F_RG,
    F_RGB,
    F_BGR,
    F_BGRA,
    F_RGBA,
    F_DEPTH,
    F_DEPTH32,
  };

  enum Filter {
    //Both are valid for minification and magnification:
    F_NEAREST,
    F_LINEAR,

    /* Valid ONLY for minification:
     *   Chooses the mipmap that most closely matches the size of the  
     *   pixel being textured and uses the GL_NEAREST criterion (the  
     *   textureelement closest to the specified texture coordinates) to  
     *   produce a texture value.
     */
    F_NEAREST_MIPMAP_NEAREST,

    /* Valid ONLY for minification:
     *   Chooses the mipmap that most closely matches the size of the 
     *   pixel being textured and uses the GL_LINEAR criterion (a 
     *   weighted average of the four texture elements that are closest
     *   to the specified texture coordinates) to produce a texture value.
     */
    F_LINEAR_MIPMAP_NEAREST,

    /* Valid ONLY for minification:
     *   Chooses the two mipmaps that most closely match the size of the 
     *   pixel being textured and uses the GL_NEAREST criterion (the 
     *   texture element closest to the specified texture coordinates)  
     *   to produce a texture value from each mipmap. The final texture 
     *   value is a weighted average of those .
     */
    F_NEAREST_MIPMAP_LINEAR,

    /* Valid ONLY for minification:
     *   Chooses the two mipmaps that most closely match the size of the 
     *   pixel being textured and uses the GL_LINEAR criterion (a 
     *   weighted average of the texture elements that are closest to 
     *   the specified texture coordinates) to produce a texture value 
     *   from each mipmap.
     *   The final texture value is a weighted average of those two 
     *   values.
    */
    F_LINEAR_MIPMAP_LINEAR,
  };

  enum Wrap {
    W_REPEAT,
    W_MIRRORED_REPEAT,
    W_CLAMP_TO_EDGE,
  };

  // Uploading Data to the texture ------------------
  /*
   * @param f: data format of *data.
   * @param t: data type of each element of *data.
   * @param data: the data itself.
   * @param mipmap_LOD: the LOD to fill (mipmapping only).
   */
  virtual void set_data(const Format f, const EDK::Type t,
                        const void* data, 
                        const unsigned int mipmap_LOD = 0) = 0;

  // Use the texture in the given texture unit:
  virtual void bind(unsigned int textUnit) const = 0;
  virtual const GLuint internal_id() const = 0;

  //Other functions that must be implemented.
  //Remember to call the parent function in order to set the value.
  __forceinline virtual void set_min_filter(Filter f) { min_filter_ = f; }
  __forceinline virtual void set_mag_filter(Filter f) { mag_filter_ = f; }
  __forceinline virtual void set_wrap_s(Wrap c) { wrap_s_ = c; }
  __forceinline virtual void set_wrap_t(Wrap c) { wrap_t_ = c; }
  __forceinline virtual void set_wrap_r(Wrap c) { wrap_r_ = c; }
  __forceinline virtual void generateMipmaps() const { 
    // Needed to implement if filtering uses mipmapping.
  }

  // -------------------------------------------------------------------
  void set_wrap(Wrap c) { set_wrap_s(c); set_wrap_t(c); set_wrap_r(c); }
  __forceinline const Filter min_filter() const { return min_filter_; }
  __forceinline const Filter mag_filter() const { return mag_filter_; }
  __forceinline const Wrap wrap_s() const { return wrap_s_; }
  __forceinline const Wrap wrap_t() const { return wrap_t_; }
  __forceinline const Wrap wrap_r() const { return wrap_r_; }
  __forceinline const Type type() const           { return type_;   }
  __forceinline const Format format() const       { return format_; }
  __forceinline const unsigned int width() const  { return width_;  }
  __forceinline const unsigned int height() const { return height_; }
  __forceinline const unsigned int depth() const  { return depth_;  }
  // -------------------------------------------------------------------

  //Formats: .png, .jpg, .jpeg, .tga, .bmp, .psd, .gif, .hdr, .pic
  static const bool Load(const char *filename, 
                         EDK::ref_ptr<Texture> *output_tex);

 protected:
  Texture() : type_(T_Invalid), format_(F_None), 
              width_(0), height_(0), depth_(0) {}

  void init(const Type t, const Format internal_format, 
            const unsigned int width, const unsigned int height = 1, 
            const unsigned int depth = 1) {
    type_ = t;
    format_ = internal_format;
    width_ = width;
    height_ = height;
    depth_ = depth;
  }
    
  //Referenced requires virtual protected destructor:
  virtual ~Texture() {}

 private:
  Type type_;
  Format format_;
  unsigned int width_;
  unsigned int height_;
  unsigned int depth_;
  Filter min_filter_;
  Filter mag_filter_;
  Wrap wrap_s_;
  Wrap wrap_t_;
  Wrap wrap_r_;

  Texture(const Texture&);
  Texture& operator=(const Texture&);
}; //Texture

} //EDK

#endif //__EDK_TEXTURE_H__
