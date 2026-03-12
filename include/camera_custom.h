/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom fly camera header file.
 *
 */


#ifndef __CAMERA_CUSTOM_H__
#define __CAMERA_CUSTOM_H__ 1

#include "EDK/edk_camera.h"
#include "ESAT/math.h"


namespace EDK {

class CameraCustom : public EDK::Camera {
 public:
  CameraCustom();

  void initViewTarget(const float window_width,
                      const float window_height);

  void update(const double dt);

  void setEnabled(const bool enabled);
  void setSpeed(const float speed);
  void setSensitibity(const float sensitivity);

  const bool enabled() const;
  const float speed() const;
  const float sensitivity() const;
  esat::Vec3 direction() const;

  //TODO something in the future:
  //virtual void doCull(const Node* root_node) override;
  //virtual void doRender() const override;

 protected:
  virtual ~CameraCustom();

  //TODO anything with these vars, these vars can be changed:
  bool enabled_;
  float speed_;
  float sensitivity_;
  esat::Vec3 view_dir_;
  esat::Vec2 window_size_;
  esat::Vec2 accumulated_delta_mouse_;

 private:
  CameraCustom(const CameraCustom&);
  //CameraCustom(CameraCustom&&);
  CameraCustom& operator=(const CameraCustom&);
};

} //EDK

#endif //__CAMERA_CUSTOM_H__
