/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Diffuse material class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */

#ifndef __EDK_MAT_DIFFUSE_H__
#define __EDK_MAT_DIFFUSE_H__

#include <cstring>
#include "edk_ref_ptr.h"
#include "edk_material.h"
#include "edk_material_settings.h"
#include "dev/edk_program.h"


namespace EDK {

class MatDiffuse : public Material {
 public:
  MatDiffuse();
  virtual const bool enable(const MaterialSettings *) const;
  virtual void setupCamera(const float projection[16], 
                           const float view[16]) const;
  virtual void setupModel(const float m[16]) const;

  virtual unsigned int num_attributes_required() const;
  virtual Attribute attribute_at_index(const unsigned int idx) const;
  virtual Type attribute_type_at_index(const unsigned int idx) const;

  class Settings : public MaterialSettings {
   public:
    Settings() { for(int i = 0; i < 4; ++i) data_[i] = 1.0f; }
    void set_color(const float v[4]) { memcpy(data_, v, sizeof(data_)); }
    void set_color(float r, float g, float b, float a = 1.0f) {
      data_[0] = r; data_[1] = g; data_[2] = b; data_[3] = a;
    }
    const float *color() const { return data_; }
   protected:
    virtual ~Settings() {}
   private:
    Settings(const Settings&);
    Settings& operator=(const Settings&);
    float data_[4];
  }; //Settings

 protected:
  virtual ~MatDiffuse() {}

 private:
  ref_ptr<dev::Program> program_;
  MatDiffuse(const MatDiffuse&);
  MatDiffuse& operator=(const MatDiffuse &);
}; //MatDiffuse

} //EDK

#endif //__EDK_MAT_DIFFUSE_H__
