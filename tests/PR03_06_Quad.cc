/*
 * 
 * Ivan Sancho as Unreal Authorized Instructor, 2022-23
 * Graphics Programming
 * 
 * PR03_00_Template.cc
 * 
 */


#include <stdio.h>
#include <math.h>
#include "ESAT/window.h"
#include "ESAT/input.h"
#include "ESAT/draw.h"
#include "ESAT/time.h"

#include "EDK/edk_geometry.h"
#include "EDK/edk_camera.h"
#include "EDK/edk_drawable.h"
#include "EDK/edk_material_diffuse_texture.h"
#include "EDK/edk_material_diffuse.h"
#include "EDK/edk_texture.h"
#include "EDK/dev/edk_gpu_manager.h"
#include "geometry_custom_quad.h"
#include "ESAT_extra/imgui.h"
#include "EDK/dev/edk_opengl.h"

#include "material_custom.h"



//Unnamed struct and it's unique instance:
struct {
  EDK::ref_ptr<EDK::Camera> camera;
  EDK::ref_ptr<EDK::Node> root;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;


void InitScene() {
  //Allocating root node:
  EDK::Node* root = GameState.root.alloc();

  //Material
  //EDK::ref_ptr<EDK::MatDiffuse> mat;
  //mat.alloc();

  //Material settings
  /*EDK::ref_ptr<EDK::MatDiffuse::Settings> mat_set;
  mat_set.alloc();
  float color[4] = {1.0f, 0.0f, 1.0f, 1.0f};
  mat_set->set_color(color);
  */
  
  EDK::ref_ptr<EDK::Texture> texture;
  EDK::Texture::Load("./test/T_Postpro.png", &texture);
  if (!texture) {
      printf("Can't load texture.png\n");
      exit(-2);
  }

  EDK::ref_ptr<EDK::Texture> texture2;
  EDK::Texture::Load("./test/T_EDK_Logo.png", &texture2);
  if (!texture2) {
      printf("Can't load texture.png\n");
      exit(-2);
  }
  EDK::ref_ptr<EDK::Texture> texture3;
  EDK::Texture::Load("./test/T_Rainbow.png", &texture3);
  if (!texture3) {
      printf("Can't load texture.png\n");
      exit(-2);
  }

  //Geometry
  EDK::ref_ptr<EDK::QuadCustom> quad;
  quad.alloc();
  quad->init();

  EDK::ref_ptr<EDK::MaterialCustom> quad_mat;
  quad_mat.alloc();
  quad_mat->init("./test/vertex3.vs", "./test/fragment3.fs");
  EDK::ref_ptr<EDK::MaterialCustom::MaterialTextureCustomSettings> quad_mat_set;
  quad_mat_set.alloc();

  quad_mat_set->set_texture(texture, 0);
  quad_mat_set->set_texture(texture2, 1);
  quad_mat_set->set_texture(texture3, 2);

  //Drawable
  EDK::ref_ptr<EDK::Drawable> drawable;
  drawable.alloc();
  drawable->set_geometry(quad.get());
  drawable->set_material(quad_mat.get());
  drawable->set_material_settings(quad_mat_set.get());

  root->addChild(drawable.get());

  //Allocating and initializing the camera:
  GameState.camera.alloc();
  float pos[] = { 0.0f, 3.0f, 4.0f };
  float target[] = { 0.0f,0.0f,0.0f };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_target(target);
  GameState.camera->setupPerspective(70.0f, 8.0f / 6.0f, 1.0f, 1500.0f);
  EDK::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  GameState.camera->set_clear_color(0.94f, 1.0f, 0.94f, 1.0f);
}

void RenderFn() {
  //For every frame... determine what's visible:
  GameState.camera->doCull(GameState.root.get());

  //Rendering the scene:
  EDK::dev::GPUManager::CheckGLError("begin Render-->");
  GameState.camera->doRender();
  EDK::dev::GPUManager::CheckGLError("end Render-->");
}

void ImGuiFn(double dt) {
  ImGui::Begin("FPS Window");
  ImGui::Text("FPS: %0.1f", 1000.0 / dt);
  ImGui::Text("Delta time: %0.3f ms", dt);
  ImGui::End();
  ImGui::Render();
}

int esat::main(int argc, char** argv) {
  esat::WindowInit(kWindowWidth, kWindowHeight);
  InitScene();
  double dt = 0.0;
  double last_time = esat::Time();
  while (!esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) &&
         esat::WindowIsOpened()) {
    UpdateFn();
    RenderFn();
    dt = esat::Time() - last_time;
    last_time = esat::Time();
    ImGuiFn(dt);
    esat::WindowFrame();
  }
  return 0;
}
