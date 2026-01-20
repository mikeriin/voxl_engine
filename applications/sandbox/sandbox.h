#ifndef SANDBOX_H
#define SANDBOX_H


#include "command_manager.h"
#include "dev_console.h"
#include "res/shader_program.h"
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
        _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::INFO,
          .buffer = std::format("You used /test with {} arg(s).", args.size())
        });
        return true;
      }
    });


    std::array<voxl::ShaderCreateInfo, 2> shaders;
    shaders[0] = { .type = voxl::ShaderType::VERTEX, .path = "global.vert" };
    shaders[1] = { .type = voxl::ShaderType::FRAGMENT, .path = "global.frag" };

    voxl::ShaderProgramCreateInfo programInfo{};
    programInfo.name = "sh_global";
    programInfo.shaders = shaders;
    
    voxl::Loader<voxl::ShaderProgram>::DefineRelativePath("assets/shaders/");
    auto global_shader_program = _pCtx->pResManager->Load<voxl::ShaderProgram>(programInfo.name, programInfo); // cpu side, reste en cache
    _pCtx->pRenderer->Device()->CreateShaderProgram(global_shader_program.get()); // gpu side

    _pCtx->pCmdManager->RegisterCommand("reload_shader", voxl::Command{
      .helper = "/reload_shader <name>",
      .func = [this](std::span<const std::string_view> args) -> bool {
        if (args.size() != 1) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::WARNING,
            .buffer = std::format("Failed to execute /reload_shader: too many arguments, excpected 1.")
          });
          return false;
        }

        std::string name = std::string(args[0]);
        auto shader_to_reload = _pCtx->pResManager->Get<voxl::ShaderProgram>(name);
        if (!shader_to_reload) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::ERROR,
            .buffer = std::format("Failed to execute /reload_shader: this shader doesn't exist.")
          });
          return false;
        }

        voxl::Loader<voxl::ShaderProgram>::Reload(shader_to_reload.get());

        if (!_pCtx->pRenderer->Device()->ReloadShaderProgram(shader_to_reload.get())) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::ERROR,
            .buffer = std::format("Failed to execute /reload_shader: failed to send shaders to the GPU.")
          });
          return false;
        }

        _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::INFO,
          .buffer = std::format("Successfully reloaded {}.", name)
        });

        return true;
      }
    });


    _pCtx->pCmdManager->RegisterCommand("help", voxl::Command{
      .helper = "/help",
      .func = [this](std::span<const std::string_view> args) -> bool {
        if (args.size() == 0) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::NONE,
            .buffer = _pCtx->pCmdManager->Help()
          });
          return true;
        }

        if (args.size() != 1) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::WARNING,
            .buffer = std::format("Failed to execute /help: too many arguments, excpected 0 or 1.")
          });
          return false;
        } 

        _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::NONE,
          .buffer = std::format("{} --> {}", args[0], _pCtx->pCmdManager->Helper(args[0]))
        });
        return true;
      }
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