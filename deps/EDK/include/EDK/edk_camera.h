/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Camera utilities.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_CAMERA_H__
#define __EDK_CAMERA_H__

#include "edk_referenced.h"
#include "edk_ref_ptr.h"
#include "edk_scoped_ptr.h"
#include <vector>


namespace EDK {

class Node;
class RenderTarget;
class Material;
class MaterialSettings;


class Camera : public virtual Referenced {
 public:
  Camera();

  //Main methods:
  void setupPerspective(const float fovy, const float aspect, 
                        const float znear, const float zfar);
  void setupOrtho(const float left, const float right,
                  const float bottom, const float top,
                  const float znear, const float zfar);
  void setupFrustum(const float left, const float right,
                    const float bottom, const float top,
                    const float znear, const float zfar);

  //Setters:
  void set_position(const float pos[3]);
  void set_view_direction(const float pos[3]);
  void set_view_target(const float pos[3]);
  void set_clear_flags(const bool color, const bool depth, 
                       const bool stencil);
	void set_clear_color(const float r, const float g, 
                       const float b, const float a);

  //Window coordinates to object coordinates:
  void unproject(const float normalized_win_x, 
                 const float normalized_win_y, 
                 const float z_between_near_far, float output[3]) const;

	
  //Getters:
  const float *position() const;
  const float *target() const;
  const float *up_direction() const;
  const float *projection_matrix() const;
  const float *view_matrix() const;

  //Render methods:
  //Determine which objects are visible:
  virtual void doCull(const Node *root_node);
  //Renders all visible (not culled) objects:
  virtual void doRender() const;

  /** Renders all visible (not culled) objects with the given material /material-settings */
  virtual void doRenderWithMaterial(const Material *mat, 
                                    const MaterialSettings *ms) const;

  //Hidden camera structure data:
  struct Data;

 protected:
  virtual ~Camera();

 private:
  EDK::scoped_ptr<Data> data_;
  Camera(const Camera&);
  Camera& operator=(const Camera&);
}; //Camera

} //EDK

#endif //__EDK_CAMERA_H__
