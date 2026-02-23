/*
 * 
 * Ivan Sancho as Unreal Authorized Instructor, 2022-23
 * Graphics Programming
 * 
 * PR03_01_Demo.cc
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
#include "EDK/edk_texture.h"
#include "EDK/dev/edk_gpu_manager.h"

#include "ESAT_extra/imgui.h"
#include "EDK/dev/edk_opengl.h"


//Unnamed struct and it's unique instance:
struct {
  EDK::ref_ptr<EDK::Camera> camera;
  EDK::ref_ptr<EDK::Node> root;
} GameState;

const int kWindowWidth = 1024;
const int kWindowHeight = 768;

int degrees = 0;


void InitScene() {
  //Allocating root node:
  EDK::Node* root = GameState.root.alloc();  
  
  //Material, Material Settings
  EDK::ref_ptr<EDK::Texture> cube_texture;
  EDK::Texture::Load("./test/T_Chopper.jpg",&cube_texture); //alloc inside
  EDK::ref_ptr<EDK::MatDiffuseTexture> cube_mat;
  cube_mat.alloc();
  EDK::ref_ptr<EDK::MatDiffuseTexture::Settings> cube_mat_set;
  cube_mat_set.alloc();
  float color[4] = { 1.0f,1.0f,1.0f,1.0f };
  cube_mat_set->set_color(color);
  cube_mat_set->set_texture(cube_texture.get());

  EDK::ref_ptr<EDK::Texture> cube_texture2;
  EDK::Texture::Load("./test/T_Postpro.png", &cube_texture2); //alloc inside
  EDK::ref_ptr<EDK::MatDiffuseTexture> cube_mat2;
  cube_mat2.alloc();
  EDK::ref_ptr<EDK::MatDiffuseTexture::Settings> cube_mat_set2;
  cube_mat_set2.alloc();
  cube_mat_set2->set_color(color);
  cube_mat_set2->set_texture(cube_texture2.get());

  //Geometry
  EDK::ref_ptr<EDK::Geometry> cube_geo;

  EDK::scoped_array<EDK::ref_ptr<EDK::Geometry>> geometries;
  EDK::scoped_array<char> error_log;
  EDK::LoadObj("./test/SM_Suzanne.obj", &geometries, &error_log);
  
  EDK::CreateCube(&cube_geo);

  //Drawable
  EDK::ref_ptr<EDK::Drawable> cube_drawable;
  cube_drawable.alloc();
  cube_drawable->set_geometry(cube_geo.get());
  cube_drawable->set_material(cube_mat.get());
  cube_drawable->set_material_settings(cube_mat_set.get());
  float pos_cube[3] = { 2.0f,0.0f,0.0f};
  cube_drawable->set_position(pos_cube);

  //Drawable

  EDK::ref_ptr<EDK::Drawable> cube_drawable2;
  cube_drawable2.alloc();
  cube_drawable2->set_geometry(cube_geo.get());
  cube_drawable2->set_material(cube_mat2.get());
  cube_drawable2->set_material_settings(cube_mat_set2.get());
  float pos_cube2[3] = { -2.0f,0.0f,0.0f };
  cube_drawable->set_position(pos_cube2);

  root->addChild(cube_drawable.get());
  root->addChild(cube_drawable2.get());

  for (int i = 0; i < geometries.size(); i++) {
      EDK::ref_ptr<EDK::Drawable> suzane_drawable;
      suzane_drawable.alloc();
      suzane_drawable->set_geometry(geometries[i].get());
      suzane_drawable->set_material(cube_mat2.get());
      suzane_drawable->set_material_settings(cube_mat_set2.get());
      float pos_cube_tmp[3] = {0.0f,2.0f,0.0f };
      suzane_drawable->set_position(pos_cube_tmp);      

      root->addChild(suzane_drawable.get());
  }

  //Allocating and initializing the camera:
  float pos[] = { 0.0f, 4.0f,6.0f };
  float target[] = { 0.0f, 0.0f, 0.0f };
  float ar = (float) kWindowWidth / (float) kWindowHeight;
  GameState.camera.alloc();
  GameState.camera->set_position(pos);
  GameState.camera->set_view_target(target);
  GameState.camera->setupPerspective(60.0f, ar, 0.1f, 100.0f);
  GameState.camera->set_clear_color(1.0f, 1.0f, 1.0f, 1.0f);
  EDK::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  //Updates the root node:
  float speed = -15.0f;
  float radius = 3.0f;

  for (int i = 0; i < GameState.root->num_children(); i++) {
      if (i >= 2) {
          GameState.root->child(i)->set_rotation_y(esat::Time() * 0.001f * speed);
      }
      else if (i == 0) {
          GameState.root->child(i)->set_position(sin(degrees * 0.0174444)*radius,0.0f, cos(degrees * 0.0174444)* radius);
      }
      else if (i == 1) {
          GameState.root->child(i)->set_position(sin((degrees+180) * 0.0174444)* radius, 0.0f, cos((degrees + 180) * 0.0174444)* radius);
      }
      degrees++;
  }

  //Orbital camera:
  
  /*double mx = esat::MousePositionX();
  double my = esat::MousePositionY();
  double p = sin(-my / 200.0f) ;
  float pos[] = { 0.0f,0.0f,3.5 };
  float view[] = { -pos[0], -pos[1], -pos[2] };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  GameState.camera->set_clear_color(0.9f, 1.0f, 0.9f, 1.0f);
  */
  
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
