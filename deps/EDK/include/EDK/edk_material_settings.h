/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * Base Material Settings class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */

#ifndef __EDK_MATERIAL_SETTINGS_H__
#define __EDK_MATERIAL_SETTINGS_H__

#include "edk_referenced.h"


namespace EDK {

class MaterialSettings : public virtual Referenced {
 protected:
  MaterialSettings() {}
  virtual ~MaterialSettings() {}

 private:
  MaterialSettings(const MaterialSettings&);
  MaterialSettings& operator=(const MaterialSettings &);
}; //MaterialSettings

} //EDK

#endif //__EDK_MATERIAL_SETTINGS_H__
