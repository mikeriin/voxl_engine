#ifndef SANDBOX_H
#define SANDBOX_H


#include <array>
#include <format>
#include <print>
#include <string>

#include <voxl.h>


class SandboxLayer: public voxl::Layer {
public:
  void OnAttach(voxl::AppContext& ctx) override { 
    _pCtx = &ctx;

    _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
      .level = voxl::DevConsoleMessage::INFO,
      .buffer = "SandboxLayer attached"
    });

    _pCtx->pCmdManager->RegisterCommand("test", voxl::Command{
      .helper = "/test <args>",
      .func = [this](std::span<const std::string_view> args) -> bool {
        this->_pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::INFO,
          .buffer = std::format("You used /test with {} arg(s).", args.size())
        });
        return true;
      }
    });


    std::array<voxl::ShaderCreateInfo, 2> shaders;
    shaders[0] = { .type = voxl::ShaderType::VERTEX, .path = ".vert" };
    shaders[1] = { .type = voxl::ShaderType::FRAGMENT, .path = ".frag" };

    voxl::ShaderProgramCreateInfo programInfo{};
    programInfo.name = "sh_global";
    programInfo.shaders = shaders;
    
    auto global_shader_program = _pCtx->pResManager->Load<voxl::ShaderProgram>(programInfo.name, programInfo);
    _pCtx->pRenderer->Device()->CreateShaderProgram(global_shader_program.get());
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