/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Postprocess material class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */

#ifndef __EDK_MAT_POSTPROCESS_H__
#define __EDK_MAT_POSTPROCESS_H__

#include "edk_ref_ptr.h"
#include "edk_material.h"
#include "edk_material_settings.h"
#include "edk_texture.h"
#include "dev/edk_program.h"


namespace EDK {

class MatPostprocess : public Material {
 public:
  MatPostprocess();
  virtual const bool enable(const MaterialSettings *) const;
  virtual void setupCamera(const float projection[16], 
                           const float view[16]) const;
  virtual void setupModel(const float m[16]) const;

  unsigned int num_attributes_required() const;
  Attribute attribute_at_index(const unsigned int idx) const;
  Type attribute_type_at_index(const unsigned int idx) const;

  class Blit: public MaterialSettings {
   public:
    Blit();
    void set_multiply_color(const float v[4]);
    void set_additive_color(const float v[4]);
    void set_texture(Texture *tex) { texture_ = tex; }
   protected:
    virtual ~Blit() {}
   private:
    Blit(const Blit&);
    Blit& operator=(const Blit&);
    float m_color_[4];
    float a_color_[4];
    ref_ptr<Texture> texture_;
    friend class MatPostprocess;
  }; //Blit

  class Fade: public MaterialSettings {
   public:
    Fade() : progress_(0.0f) {}
    void set_textures(Texture *A, Texture *B) { texture_[0] = A; texture_[1] = B; }
    void set_progress(float f) { progress_ = f; }
   protected:
    virtual ~Fade() {}
   private:
    Fade(const Fade&);
    Fade& operator=(const Fade&);
    float progress_;
    ref_ptr<Texture> texture_[2];
    friend class MatPostprocess;
  }; //Fade

  class Blur : public MaterialSettings {
   public:
    enum Direction {
      kHorizontal,
      kVertical,
    };
    Blur() :direction_(kHorizontal) {}
    void set_texture(Texture *t) { texture_ = t; }
    void set_direction(Direction d) { direction_ = d; }
   protected:
    virtual ~Blur() {}
   private:
    Blur(const Blur&);
    Blur& operator=(const Blur&);
      
    ref_ptr<Texture> texture_;
    Direction direction_;

    friend class MatPostprocess;
  }; //Blur

 protected:
  virtual ~MatPostprocess() {}

 private:
  ref_ptr<dev::Program> program_;
  MatPostprocess(const MatPostprocess&);
  MatPostprocess& operator=(const MatPostprocess &);
}; //MatPostprocess

} //EDK

#endif //__EDK_MAT_POSTPROCESS_H__
