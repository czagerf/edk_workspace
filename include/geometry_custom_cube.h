 /**
  *
  * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
  *
  * Custom cube header file.
  *
  */

#ifndef __GEOMETRY_CUSTOM_CUBE_H__
#define __GEOMETRY_CUSTOM_CUBE_H__ 1

#include "EDK/edk_geometry.h"
#include "EDK/dev/edk_buffer.h"

namespace EDK {

class CubeCustom : public EDK::Geometry {
 public:
  CubeCustom();

  void init8v(const float cube_size = 1.0f);
  void init24v(const float cube_size = 1.0f);
  virtual const bool bindAttribute(const Attribute a,
                   unsigned int where_to_bind_attribute) const override;
  virtual void render() const override;

 protected:
  virtual ~CubeCustom();

 private:
  CubeCustom(const CubeCustom&);
  //CubeCustom(CubeCustom&&);
  CubeCustom& operator=(const CubeCustom&);

  EDK::ref_ptr<EDK::dev::Buffer> elements_buffer;
  EDK::ref_ptr<EDK::dev::Buffer> order_buffer;
  bool is_initialized_;
};

} //EDK

#endif //__GEOMETRY_CUSTOM_CUBE_H__
