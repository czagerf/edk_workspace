/*
 *
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * GPUManager class.
 *
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 *
 */


#ifndef __EDK3_GPU_MANAGER_H__
#define __EDK3_GPU_MANAGER_H__

#include "EDK/edk_ref_ptr.h"
#include "EDK/edk_scoped_array.h"
#include "EDK/edk_constants.h"
#include "EDK/edk_texture.h"


namespace EDK {

class Engine;

namespace dev {

class Shader;
class Program;
class Buffer;
class Framebuffer;


class GPUManager {
 public:
  //Singleton pattern:
  static GPUManager *Instance(); 

  //Replace GPU Manager implementation.
  //Must be done before creating the window (WindowInit).
  static void ReplaceGPUManagerImplementation(GPUManager *g);

  //Called by EDK once render context is set and ready to start drawing:
  virtual void onStart() {}

  //Called by EDK once at before shuting down the render context:
  virtual void onFinish() {}

  //Graphic resources allocators:
  virtual void newShader(EDK::ref_ptr<Shader> *output);
  virtual void newProgram(EDK::ref_ptr<Program> *output);
  virtual void newBuffer(EDK::ref_ptr<Buffer> *output);
  virtual void newFramebuffer(EDK::ref_ptr<Framebuffer>* output);

  virtual void newTexture1D(const Texture::Format internal_format, 
                            const unsigned int size, 
                            EDK::ref_ptr<Texture> *output);
  virtual void newTexture2D(const Texture::Format internal_format, 
                            const unsigned int width, 
                            const unsigned int height, 
                            EDK::ref_ptr<Texture> *output);
  virtual void newTexture3D(const Texture::Format internal_format, 
                            const unsigned int width, 
                            const unsigned int height, 
                            const unsigned int depth, 
                            EDK::ref_ptr<Texture> *output);

  //Framebuffer settings:
  enum FrameBufferElement {
    kFrameBufferColor   = 1 << 0,
    kFrameBufferDepth   = 1 << 1,
    kFrameBufferStencil = 1 << 2,
  };

  virtual void clearFrameBuffer(const float rgba[4],
                                const unsigned int FBOorMask =
                                kFrameBufferColor | kFrameBufferDepth);

  //Geometry rendering:
  virtual void enableVertexAttribute(const Buffer *buffer,
                                     const unsigned int attr_index,
                                     const Type type,
                                     const bool normalized = false,
                                     const unsigned int offset = 0, 
                                     const unsigned int stride = 0);

  virtual void disableVertexAttribute(const unsigned int attrib_index);

  enum DrawMode {
    kDrawMode_Triangles,
    kDrawMode_TriangleStrip,
    kDrawMode_Points,
    kDrawMode_Lines,
    kDrawMode_LineStrip,
    kDrawMode_LineLoop
  };

  virtual void drawElements(const DrawMode mode, 
                            unsigned int count,
                            const Buffer *buffer,
                            const Type element_type = T_USHORT,
                            const unsigned int offset = 0) const;

  //Depth test:
  enum CompareFunc {
    kCompareFunc_Never,          // false -> always fails
    kCompareFunc_Less,           // a < b
    kCompareFunc_LessOrEqual,    // a <= b
    kCompareFunc_Equal,          // a == b
    kCompareFunc_NotEqual,       // a != b
    kCompareFunc_GreaterOrEqual, // a >= b
    kCompareFunc_Greater,        // a > b
    kCompareFunc_Always          // true -> always passes
  };

  virtual void enableDepthTest(const CompareFunc f);
  virtual void disableDepthTest();

  //Blending:
  enum BlendParam {
    kBlendParam_Zero,
    kBlendParam_One,
    kBlendParam_SourceColor,
    kBlendParam_OneMinusSourceColor,
    kBlendParam_DestinationColor,
    kBlendParam_OneMinusDestinationColor,
    kBlendParam_SourceAlpha,
    kBlendParam_OneMinusSourceAlpha,
    kBlendParam_DestinationAlpha,
    kBlendParam_OneMinusDestinationAlpha,
    kBlendParam_ConstantColor,
    kBlendParam_OneMinusConstantColor,
    kBlendParam_ConstantAlpha,
    kBlendParam_OneMinusConstantAlpha,
    kBlendParam_SourceAlphaSaturate,
  };

  enum BlendOp {
    kBlendOp_Add,
    kBlendOp_Subtract,
    kBlendOp_ReverseSubtract,
    kBlendOp_Min,
    kBlendOp_Max,
  };

  virtual void enableBlend(BlendParam source, BlendParam destination, 
                           BlendOp operation, 
                           const float ConstantColor[4]);
  void disableBlend();
    
  //Culling methods:
  enum FaceType {
    kFace_Back,
    kFace_Front,
    kFace_FrontAndBack
  };

  virtual void enableCullFaces(const FaceType f);
  virtual void disableCullFaces();

  //Scissor:
  virtual void enableScissor(int x, int y,
                             unsigned int width,
                             unsigned int height);
  virtual void disableScissor();

  virtual void changeColorMask(const bool red = true,
                               const bool green = true,
                               const bool blue = true,
                               const bool alpha = true);
  virtual void changeDepthMask(bool enable = true);

  virtual void bindDefaultFramebuffer();

  static void CheckGLError(const char *msg = 0L);
  static void EnableCheckGLError(bool v);

  // Called by window manager when gpu-manager needs to be initialized
  // Do not call manually.
  void doOnStart();
  void doOnFinish();

  void saveState(EDK::scoped_array<char>* output);
  void restoreState(const void* mem_block);

 protected:
  GPUManager();
  virtual ~GPUManager();

 private:
  GPUManager(const GPUManager&);
  GPUManager& operator=(const GPUManager&);
  static GPUManager *default_;
}; //GPUManager

} //dev

} //EDK

#endif //__EDK3_GPU_MANAGER_H__
