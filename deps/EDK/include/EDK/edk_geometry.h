/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Geometry class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK_GEOMETRY_H__
#define __EDK_GEOMETRY_H__

#include "edk_constants.h"
#include "edk_referenced.h"
#include "edk_ref_ptr.h"
#include "edk_scoped_array.h"


namespace EDK {
  
class Material;

class Geometry : public Referenced {
 public:
  virtual const bool bindAttribute(const Attribute a,
                                   const unsigned int att_pos) const = 0;
  virtual void render() const = 0;

  //Draws the geometry (binding the attributes needed first) for the 
  //given material which must be the current (enabled) material in use.
  void bindAttributesAndRenderWithCurrentMaterial(const Material *m) const;


 protected:
  Geometry();
  virtual ~Geometry();

 private:
  Geometry(const Geometry&);
  Geometry& operator=(const Geometry &);
}; //Geometry


void CreateCube(EDK::ref_ptr<Geometry> *output,
                const float cube_size = 1.0f,
                const bool build_normals = true,
                const bool build_uv = true);

void CreateQuad(EDK::ref_ptr<Geometry> *output,
                const float width = 1.0f,
                const float height = 1.0f,
                const bool build_normals = true,
                const bool build_uv = true);

bool LoadObj(const char *filename,
             EDK::scoped_array<EDK::ref_ptr<Geometry> > *geometries,
             EDK::scoped_array<char> *error_log);

} //EDK

#endif //__EDK_GEOMETRY_H__
