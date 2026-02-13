/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Base material class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */

#ifndef __EDK_MATERIAL_H__
#define __EDK_MATERIAL_H__

#include "edk_referenced.h"
#include "edk_constants.h"


namespace EDK {

class MaterialSettings;
class Drawable;

class Material : public virtual Referenced {
 public:
  //"True" if the material can be set with the given material settings:
  virtual const bool enable(const MaterialSettings *) const = 0;
  //Called after "enable" to setup the camera matrices:
  virtual void setupCamera(const float projection[16], 
                           const float view[16]) const = 0;
  //Called after "enable" to setup the model matrix:
  virtual void setupModel(const float model[16]) const = 0;
    
  virtual unsigned int num_attributes_required() const = 0;
  virtual Attribute attribute_at_index(const unsigned int idx) const = 0;
  virtual Type attribute_type_at_index(const unsigned int idx) const = 0;

  //Draw a full screen quad with the given mat. and mat. settings:
  void drawFullScreenQuad(const MaterialSettings *ms) const;

  //Draws a portion of the screen with the given mat. and mat. settings.
  //min_x, min_y, max_x, max_y are relative to current window, in the 
  //range from 0.0 to 1.0.
  void drawRelativeToScreenQuad(const float min_x, const float min_y,
                                const float max_x, const float max_y,
                                const MaterialSettings *mat) const;

  //Draws a portion of the screen with the given mat. and mat. settings.
  //Position is given in pixels.
  void drawScreenQuad(const float min_x, const float min_y,
                      const float max_x, const float max_y,
                      const MaterialSettings *mat) const;

 protected:
  Material() {}
  virtual ~Material() {}

 private:
  Material(const Material&);
  Material& operator=(const Material &);
}; //Material

} //EDK

#endif //__EDK_MATERIAL_H__
