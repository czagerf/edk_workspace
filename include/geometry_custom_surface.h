/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom surface revolution header file.
 *
 */


#ifndef __GEOMETRY_CUSTOM_SURFACE_H__
#define __GEOMETRY_CUSTOM_SURFACE_H__ 1

#include "EDK/edk_geometry.h"
#include "EDK/dev/edk_buffer.h"

namespace EDK {

class SurfaceCustom : public EDK::Geometry {
 public:
  SurfaceCustom();

  void init(const float* surface_points,
            const int num_heights = 4, const int num_revs = 4);
  virtual const bool bindAttribute(const Attribute a,
                       unsigned int where_to_bind_attribute) const override;
  virtual void render() const override;

 protected:
  virtual ~SurfaceCustom();

 private:
  SurfaceCustom(const SurfaceCustom&);
  //SurfaceCustom(SurfaceCustom&&);
  SurfaceCustom& operator=(const SurfaceCustom&);

  EDK::ref_ptr<EDK::dev::Buffer> elements_buffer;
  EDK::ref_ptr<EDK::dev::Buffer> order_buffer;
  bool is_initialized_;
  int num_heights_;
  int num_revs_;
};

} //EDK

#endif //__GEOMETRY_CUSTOM_SURFACE_H__
