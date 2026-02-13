/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Drawable class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_DRAWABLE_H__
#define __EDK_DRAWABLE_H__

#include "edk_node.h"
#include "edk_geometry.h"
#include "edk_material.h"
#include "edk_material_settings.h"


namespace EDK {

class Drawable : public Node {
 public:
  Drawable();
  
  __forceinline void set_visible(const bool v) { is_visible_ = v; }
  void set_material(Material *m);
  void set_material_settings(MaterialSettings *ms);
  void set_geometry(Geometry *g);

  //Getters:
  __forceinline bool visible() const { return is_visible_; }
  __forceinline const Material* material() const {
    return material_.get();
  }
  __forceinline const MaterialSettings* material_settings() const {
    return material_settings_.get();
  }
  __forceinline const Geometry* geometry() const {
    return geometry_.get();
  }

 protected:
  virtual ~Drawable();

  bool is_visible_;
  ref_ptr<Geometry> geometry_;
  ref_ptr<Material> material_;
  ref_ptr<MaterialSettings> material_settings_;

 private:
  Drawable(const Drawable&);
  Drawable& operator=(const Drawable &);
}; //Drawable

} //EDK

#endif //__EDK_DRAWABLE_H__
