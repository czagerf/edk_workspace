/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom sphere header file.
 *
 */


#ifndef __GEOMETRY_CUSTOM_SPHERE_H__
#define __GEOMETRY_CUSTOM_SPHERE_H__ 1

#include "EDK/edk_geometry.h"
#include "EDK/dev/edk_buffer.h"

namespace EDK {

class SphereCustom : public EDK::Geometry {
 public:
  SphereCustom();

  void init(const float sphere_size = 1.0f, const int num_heights = 20,
            const int num_revs = 20);
  virtual const bool bindAttribute(const Attribute a,
                       unsigned int where_to_bind_attribute) const override;
  virtual void render() const override;

 protected:
  virtual ~SphereCustom();

 private:
  SphereCustom(const SphereCustom&);
  //SphereCustom(SphereCustom&&);
  SphereCustom& operator=(const SphereCustom&);

  EDK::ref_ptr<EDK::dev::Buffer> elements_buffer;
  EDK::ref_ptr<EDK::dev::Buffer> order_buffer;
  bool is_initialized_;
  int num_heights_;
  int num_revs_;
};

} //EDK

#endif //__GEOMETRY_CUSTOM_SPHERE_H__
