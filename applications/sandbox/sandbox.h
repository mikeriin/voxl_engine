#ifndef SANDBOX_H
#define SANDBOX_H


#include <array>
#include <format>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "gfx/render_command.h"
#include "gfx/render_pipeline.h"
#include "voxl.h"


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
    shaders[0] = { .type = voxl::ShaderType::VERTEX, .path = "solid.vert" };
    shaders[1] = { .type = voxl::ShaderType::FRAGMENT, .path = "solid.frag" };
    voxl::ShaderProgramCreateInfo programInfo{};
    programInfo.name = "solid";
    programInfo.shaders = shaders;
    voxl::Loader<voxl::ShaderProgram>::DefineRelativePath("assets/shaders/");
    auto solid_shader_program = _pCtx->pResManager->Load<voxl::ShaderProgram>(programInfo.name, programInfo); // cpu side, reste en cache
    _pCtx->pDevice->CreateShaderProgram(solid_shader_program.get()); // gpu side


    shaders[0] = { .type = voxl::ShaderType::VERTEX, .path = "water.vert" };
    shaders[1] = { .type = voxl::ShaderType::FRAGMENT, .path = "water.frag" };
    programInfo.name = "water";
    programInfo.shaders = shaders;
    voxl::Loader<voxl::ShaderProgram>::DefineRelativePath("assets/shaders/");
    auto water_shader_program = _pCtx->pResManager->Load<voxl::ShaderProgram>(programInfo.name, programInfo); // cpu side, reste en cache
    _pCtx->pDevice->CreateShaderProgram(water_shader_program.get()); // gpu side

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

        if (!_pCtx->pDevice->ReloadShaderProgram(shader_to_reload.get())) {
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


    _pCtx->pCmdManager->RegisterCommand("shader_list", voxl::Command{
      .helper = "/shader_list",
      .func = [this](std::span<const std::string_view> args) -> bool {
        if (args.size() != 0) {
          _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
            .level = voxl::DevConsoleMessage::WARNING,
            .buffer = std::format("Failed to execute /shader_list: too many arguments, excpected 0.")
          });
          return false;
        }

        _pCtx->pDevConsole->Send(voxl::DevConsoleMessage{
          .level = voxl::DevConsoleMessage::NONE,
          .buffer = voxl::Loader<voxl::ShaderProgram>::GetShaderList()
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
  
    
    // création du pipeline de rendu pour les objets solides, test
    std::vector<voxl::VertexAttribute> attribs(1);
    attribs[0] = { voxl::AttribLocation::Position, voxl::AttribFormat::Float3, false };

    voxl::VertexInputLayout inputLayout{};
    inputLayout.attributes = std::move(attribs);
    inputLayout.stride = sizeof(float) * 3; // plus tard ça sera de la taille d'un vertex, pour l'instant position = 3 * float

    voxl::DepthStencilState depth{};
    depth.enableDepthTest = false;
    depth.enableDepthWrite = false;

    voxl::PipelineDesc pipelineDesc{}; // les autres valeurs sont initialisée par défaut, à voir comment modifier ça si besoin.
    pipelineDesc.program = solid_shader_program->handle;
    pipelineDesc.inputLayout = inputLayout;
    pipelineDesc.depthStencil = depth;

    _solidPipelineHandle = _pCtx->pDevice->CreatePipeline(pipelineDesc);

       
    voxl::BlendState blend{};
    blend.enableBlend = true;

    pipelineDesc.program = water_shader_program->handle;
    pipelineDesc.blend = blend;

    _waterPipelineHandle = _pCtx->pDevice->CreatePipeline(pipelineDesc);
  }

  void OnUpdate(double dt, double alpha) override {

  }

  void OnFixedUpdate(double fixed_dt) override {
    if (_pCtx->pInput->IsKeyPressed(voxl::SCANCODE_Z)) std::println("Z Pressed.");
    if (_pCtx->pInput->IsKeyHeld(voxl::SCANCODE_Z)) std::println("Z Held.");
    if (_pCtx->pInput->IsKeyReleased(voxl::SCANCODE_Z)) std::println("Z Released.");
  }

  void OnRender() override {
    voxl::CommandBuffer cmds;

    cmds.Push(voxl::CmdBindPipeline{ _solidPipelineHandle });
    // commandes pour dessiner des objets solides
    cmds.Push(voxl::CmdBindPipeline{ _waterPipelineHandle });
    // commandes pour dessiner l'eau et autres objets transparents

    _pCtx->pRenderer->SubmitRenderPass(_pCtx->pDevice, cmds);
  }

private:
  voxl::AppContext* _pCtx = nullptr;

  voxl::PipelineHandle _solidPipelineHandle;
  voxl::PipelineHandle _waterPipelineHandle;
};


#endif //! SANDBOX_H