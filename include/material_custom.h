/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Basic material header file.
 *
 */


#ifndef __MATERIAL_CUSTOM_H__
#define __MATERIAL_CUSTOM_H__ 1

#include "ESAT/math.h"
#include "EDK/edk_ref_ptr.h"
#include "EDK/edk_material.h"
#include "EDK/edk_texture.h"
#include "EDK/edk_material_settings.h"
#include "EDK/dev/edk_program.h"
#include "EDK/dev/edk_shader.h"


namespace EDK {

class MaterialCustom : public EDK::Material {
 public:
  MaterialCustom();
  void init(const char* vertex_path, const char* fragment_path);

  virtual const bool enable(const EDK::MaterialSettings *mat) const override;
  virtual void setupCamera(const float projection[16], const float view[16]) const override;
  virtual void setupModel(const float model[16]) const override;

  virtual unsigned int num_attributes_required() const;
  virtual EDK::Attribute attribute_at_index(const unsigned int attrib_idx) const;
  virtual EDK::Type attribute_type_at_index(const unsigned int attrib_index) const;


  class MaterialCustomSettings : public EDK::MaterialSettings {
   public:
    MaterialCustomSettings() {
      memset(color_, 0, sizeof(color_));
    }
    void set_color(const float v[4]) { memcpy(color_, v, sizeof(color_)); }
    const float* color() const { return color_; }
   protected:
    virtual ~MaterialCustomSettings() {}
   private:
    MaterialCustomSettings(const MaterialCustomSettings& other) {
      memcpy(color_, other.color_, sizeof(color_));
    }
    MaterialCustomSettings(MaterialCustomSettings&& other) {
      memcpy(color_, other.color_, sizeof(color_));
      memset(other.color_, 0, sizeof(color_));
    }
    MaterialCustomSettings& operator=(const MaterialCustomSettings& other) {
      memcpy(color_, other.color_, sizeof(color_));
      return *this;
    }
    float color_[4];
  }; //MaterialCustomSettings


  class MaterialTextureCustomSettings : public EDK::MaterialSettings {
  public:
      MaterialTextureCustomSettings() {}
      void set_texture(EDK::ref_ptr<EDK::Texture> t,int index) {
          texture_[index] = t;
      }
      const EDK::ref_ptr<EDK::Texture> texture(int index) const {
          return texture_[index];
      }
  protected:
      virtual ~MaterialTextureCustomSettings() {}
  private:
      MaterialTextureCustomSettings(const MaterialTextureCustomSettings& other) {
          texture_[0] = other.texture_[0];
          texture_[1] = other.texture_[1];
          texture_[2] = other.texture_[2];
      }
      MaterialTextureCustomSettings(MaterialTextureCustomSettings&& other) {
          texture_[0] = other.texture_[0];
          texture_[1] = other.texture_[1];
          texture_[2] = other.texture_[2];
          other.texture_[0].release();
          other.texture_[1].release();
          other.texture_[2].release();
      }
      MaterialTextureCustomSettings& operator=(const MaterialTextureCustomSettings& other) {
          texture_[0] = other.texture_[0];
          texture_[1] = other.texture_[1];
          texture_[2] = other.texture_[2];
          return *this;
      }
      EDK::ref_ptr<EDK::Texture> texture_[3];
  }; //MaterialCustomSettings


 protected:
  virtual ~MaterialCustom();

 private:
  EDK::ref_ptr<EDK::dev::Program> program_;

  MaterialCustom(const MaterialCustom&);
  //MaterialBasic(MaterialBasic&&);
  MaterialCustom& operator=(const MaterialCustom&);
};

} //EDK

#endif //__MATERIAL_CUSTOM_H__
