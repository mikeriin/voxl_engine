#ifndef SANDBOX_H
#define SANDBOX_H


#include <format>
#include <print>
#include <string>

#include "application.h"
#include "command_manager.h"
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

    // TODO check la validité de type des arguments et ajouter la commande à voxl.lib
    _pCtx->pCmdManager->RegisterCommand("set_clear_color", voxl::Command{
      .helper = "/set_clear_color <r> <g> <b> <a:optional>",
      .func = [this](std::span<const std::string_view> args) -> bool {
        if (args.size() == 4) {
          float r = std::stof(std::string(args[0]).c_str());
          float g = std::stof(std::string(args[1]).c_str());
          float b = std::stof(std::string(args[2]).c_str());
          float a = std::stof(std::string(args[3]).c_str());

          this->_pCtx->pRenderer->SetClearColor(
            r <= 1.0f ? r: r / 255.0f, 
            g <= 1.0f ? g: g / 255.0f, 
            b <= 1.0f ? b: b / 255.0f, 
            a <= 1.0f ? a: a / 255.0f
          );

          return true;
        }

        if (args.size() == 3) {
          float r = std::stof(std::string(args[0]).c_str());
          float g = std::stof(std::string(args[1]).c_str());
          float b = std::stof(std::string(args[2]).c_str());

          this->_pCtx->pRenderer->SetClearColor(
            r <= 1.0f ? r: r / 255.0f, 
            g <= 1.0f ? g: g / 255.0f, 
            b <= 1.0f ? b: b / 255.0f, 
            1.0f
          );

          return true;
        }

        this->_pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::INFO,
          .buffer = std::format("Failed to execute /set_clear_color, accepts only 3 or 4 arguments.", args.size())
        });
        return false;
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