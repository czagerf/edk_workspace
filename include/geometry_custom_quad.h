 /**
  *
  * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
  *
  * Custom quad header file.
  *
  */


#ifndef __GEOMETRY_CUSTOM_QUAD_H__
#define __GEOMETRY_CUSTOM_QUAD_H__ 1

#include "EDK/edk_geometry.h"
#include "EDK/dev/edk_buffer.h"

namespace EDK {

class QuadCustom : public EDK::Geometry {
 public:
  QuadCustom();

  void init(const float quad_size = 1.0f);
  virtual const bool bindAttribute(const Attribute a, 
                         unsigned int where_to_bind_attribute) 
                         const override;
  virtual void render() const override;

 protected:
  virtual ~QuadCustom();

 private:
  QuadCustom(const QuadCustom&);
  //QuadCustom(QuadCustom&&);
  QuadCustom& operator=(const QuadCustom&);

  EDK::ref_ptr<EDK::dev::Buffer> elements_buffer;
  EDK::ref_ptr<EDK::dev::Buffer> order_buffer;
  bool is_initialized_;
};

} //EDK

#endif //__GEOMETRY_CUSTOM_QUAD_H__
