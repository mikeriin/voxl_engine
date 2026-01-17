#ifndef SANDBOX_H
#define SANDBOX_H


#include <print>

#include "application.h"
#include "dev_console.h"
#include "keyboard.h"
#include "layer.h"
#include "shader_program.h"


class SandboxLayer: public voxl::Layer {
public:
  void OnAttach(voxl::AppContext& ctx) override { 
    _pCtx = &ctx;
    _pCtx->pResManager->LoadOrGet<voxl::ShaderProgram>("sh_global", "global");

    _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
      .level = voxl::DevConsoleMessage::INFO,
      .buffer = "SandboxLayer attached"
    });
  }

  void OnUpdate(double dt, double alpha) override {

  }

  void OnFixedUpdate(double fixed_dt) override {
    if (_pCtx->pInput->IsKeyPressed(voxl::SCANCODE_Z)) std::println("Z Pressed.");
    if (_pCtx->pInput->IsKeyHeld(voxl::SCANCODE_Z)) std::println("Z Held.");
    if (_pCtx->pInput->IsKeyReleased(voxl::SCANCODE_Z)) std::println("Z Released.");
  }

private:
  voxl::AppContext* _pCtx = nullptr;
};


#endif //! SANDBOX_H