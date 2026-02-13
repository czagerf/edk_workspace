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


void InitScene() {
  //Initializing the font:
  esat::DrawSetTextFont("./test/Font.ttf");
  esat::DrawSetTextSize(18);
  esat::DrawSetFillColor(255, 255, 255, 128);

  EDK::dev::GPUManager::CheckGLError("Initializing the scene...");
  
  //Creating a cube:
  EDK::ref_ptr<EDK::Geometry> quad_cube;
  EDK::CreateCube(&quad_cube, 1.0f, true, true);

  //Loading texture:
  EDK::ref_ptr<EDK::Texture> texture;
  EDK::Texture::Load("./test/T_EDK_Logo.png", &texture);
  if (!texture) {
    printf("Can't load texture.png\n");
    exit(-2);
  }

  //Initializing the material and its settings:
  EDK::ref_ptr<EDK::MatDiffuseTexture> quad_mat;
  quad_mat.alloc();
  EDK::ref_ptr<EDK::MatDiffuseTexture::Settings> quad_mat_set;
  quad_mat_set.alloc();
  float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  quad_mat_set->set_color(color);
  quad_mat_set->set_texture(texture.get());

  //Allocating root node:
  EDK::Node* root = GameState.root.alloc();

  //Creates the drawables (Geometry + Material + Settings):
  EDK::ref_ptr<EDK::Drawable> drawable;
  drawable.alloc();
  drawable->set_geometry(quad_cube.get());
  drawable->set_material(quad_mat.get());
  drawable->set_material_settings(quad_mat_set.get());
  drawable->set_position(0.0f, 0.0f, 0.0f);
  drawable->set_HPR(0.0f,0.0f,0.0f);
  root->addChild(drawable.get());

  //Allocating and initializing the camera:
  float pos[] = { -0.0f, 2.0f,-10.0f };
  float view[] = { 0.0f, 0.0f, 1.0f };
  float ar = (float) kWindowWidth / (float) kWindowHeight;
  GameState.camera.alloc();
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  GameState.camera->setupPerspective(70.0f, ar, 0.1f, 100.0f);
  GameState.camera->set_clear_color(1.0f, 1.0f, 1.0f, 1.0f);
  EDK::dev::GPUManager::CheckGLError("Prepare END");
}

void UpdateFn() {
  //Updates the root node:
  float speed = 5.0f;
  GameState.root->set_rotation_y(esat::Time() * 0.001f * speed);

  //Orbital camera:
  double mx = esat::MousePositionX();
  double my = esat::MousePositionY();
  double p = sin(-my / 200.0f) ;
  float pos[] = { (float) (p * cos(mx * 0.01)),
                 (float) ( 2.0f * cos(-my / 200.0)),
                  (float) (p * sin(mx * 0.01)) };
  float view[] = { -pos[0], -pos[1], -pos[2] };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
  GameState.camera->set_clear_color(1.0f, 1.0f, 1.0f, 1.0f);
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
