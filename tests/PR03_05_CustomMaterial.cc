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
#include "EDK/edk_material_diffuse.h"
#include "EDK/edk_material_diffuse_texture.h"
#include "EDK/edk_texture.h"
#include "EDK/dev/edk_gpu_manager.h"

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
  //Initializing the font:
  esat::DrawSetTextFont("./test/Font.ttf");
  esat::DrawSetTextSize(18);
  esat::DrawSetFillColor(255, 255, 255, 128);

  EDK::dev::GPUManager::CheckGLError("Initializing the scene...");
  
  //Creating a cube:
  EDK::ref_ptr<EDK::Geometry> cube_geo;
  EDK::CreateCube(&cube_geo);

  //Loading texture:
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

  //Allocating root node:
  EDK::Node* root = GameState.root.alloc();
  EDK::ref_ptr<EDK::Node> node_1;
  node_1.alloc();

  //Initializing the material and its settings:
  EDK::ref_ptr<EDK::MaterialCustom> cube_mat;
  cube_mat.alloc();
  cube_mat->init("./test/vertex.vs", "./test/fragment.fs");
  EDK::ref_ptr<EDK::MaterialCustom::MaterialCustomSettings> cube_mat_set;
  cube_mat_set.alloc();
  
  float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  cube_mat_set->set_color(color);

  //Creates the drawables (Geometry + Material + Settings):
  EDK::ref_ptr<EDK::Drawable> drawable;
  
  drawable.alloc();
  drawable->set_geometry(cube_geo.get());
  drawable->set_material(cube_mat.get());
  drawable->set_material_settings(cube_mat_set.get());
  drawable->set_position(-1.0f,0.0f,0.0f);
  /*drawable->set_HPR(360.0f * rand() / RAND_MAX,
              360.0f * rand() / RAND_MAX, 
              360.0f * rand() / RAND_MAX);
*/
  node_1->addChild(drawable.get());

  EDK::ref_ptr<EDK::MaterialCustom> cube_mat2;
  cube_mat2.alloc();
  cube_mat2->init("./test/vertex2.vs", "./test/fragment2.fs");
  EDK::ref_ptr<EDK::MaterialCustom::MaterialTextureCustomSettings> cube_mat2_set;
  cube_mat2_set.alloc();
  cube_mat2_set->set_texture(texture, 0);
  cube_mat2_set->set_texture(texture2, 1);
  cube_mat2_set->set_texture(texture3, 2);

  EDK::ref_ptr<EDK::Drawable> drawable2;

  drawable2.alloc();
  drawable2->set_geometry(cube_geo.get());
  drawable2->set_material(cube_mat2.get());
  drawable2->set_material_settings(cube_mat2_set.get());
  drawable2->set_position(1.0f, 0.0f, 0.0f);
  /*drawable->set_HPR(360.0f * rand() / RAND_MAX,
              360.0f * rand() / RAND_MAX,
              360.0f * rand() / RAND_MAX);
*/
  node_1->addChild(drawable2.get());

  root->addChild(node_1.get());

  //Allocating and initializing the camera:
  float pos[] = { 0.0f, 2.0f, 5.0f };
  float target[] = { 0.0f, 0.0f, 0.0f };
  float ar = (float) kWindowWidth / (float) kWindowHeight;
  GameState.camera.alloc();
  GameState.camera->set_position(pos);
  GameState.camera->set_view_target(target);
  GameState.camera->setupPerspective(70.0f, ar, 0.1f, 100.0f);
  GameState.camera->set_clear_color(1.0f,1.0f,1.0f,1.0f);
  EDK::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  //Updates the root node:
  float speed = 20.0f;

  EDK::Node* node;
  node = GameState.root->child(0);
  node->set_rotation_y(esat::Time() * 0.001f * speed);
  node->child(0)->set_rotation_y(esat::Time() * 0.001f * speed*5);
  node->child(1)->set_rotation_y(esat::Time() * 0.001f * -speed*10);

  //Orbital camera:
  /*double mx = esat::MousePositionX();
  double my = esat::MousePositionY();
  double p = sin(-my / 200.0f) * 220.0f;
  float pos[] = { (float) (p * cos(mx * 0.01)),
                  (float) (220.0 * cos(-my / 200.0)),
                  (float) (p * sin(mx * 0.01)) };
  float view[] = { -pos[0], -pos[1], -pos[2] };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  GameState.camera->set_clear_color(1.0f, 1.0f, 1.0f, 1.0f);
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
