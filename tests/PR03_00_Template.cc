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
  //Allocating root node:
  EDK::Node* root = GameState.root.alloc();

  //Allocating and initializing the camera:
  GameState.camera.alloc();
  float pos[] = { 120.0f, 140.0f, 120.0f };
  float view[] = { -120.0f, -140.0f, -120.0f };
  GameState.camera->set_position(pos);
  GameState.camera->set_view_direction(view);
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
