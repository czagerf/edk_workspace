/**
 *
 * Ivan Sancho as Unreal Authorized Instructor (UAI), 2022-23
 *
 * Custom terrain header file.
 *
 * Use of Simplex Noise algorithm:
 * https://github.com/SRombauts/SimplexNoise
 *
 */


#ifndef __GEOMETRY_CUSTOM_TERRAIN_H__
#define __GEOMETRY_CUSTOM_TERRAIN_H__ 1

#include "EDK/edk_geometry.h"
#include "EDK/dev/edk_buffer.h"

namespace EDK {

class TerrainCustom : public EDK::Geometry {
 public:
  TerrainCustom();


  //Vertices: (num_cols+1)*(num_rows+1)
  //Normal: (0,1,0)
  void init(const int num_cols = 4, const int num_rows = 4,
            const float height_mult = 1.0f, const float size = 1.0f,
            const bool is_centered = true);
  virtual const bool bindAttribute(const Attribute a,
                 unsigned int where_to_bind_attribute) const override;
  virtual void render() const override;

 protected:
  virtual ~TerrainCustom();

 private:
  TerrainCustom(const TerrainCustom&);
  //TerrainCustom(TerrainCustom&&);
  TerrainCustom& operator=(const TerrainCustom&);

  EDK::ref_ptr<EDK::dev::Buffer> elements_buffer;
  EDK::ref_ptr<EDK::dev::Buffer> order_buffer;
  bool is_initialized_;
  int num_rows_;
  int num_cols_;
};

} //EDK

#endif //__GEOMETRY_CUSTOM_TERRAIN_H__
