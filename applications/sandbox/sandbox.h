#ifndef SANDBOX_H
#define SANDBOX_H


#include <array>
#include <format>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "gfx/buffer.h"
#include "gfx/render_command.h"
#include "gfx/render_pipeline.h"
#include "gfx/render_types.h"
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
    voxl::ShaderProgramCreateInfo program_info{};
    program_info.name = "solid";
    program_info.shaders = shaders;
    voxl::Loader<voxl::ShaderProgram>::DefineRelativePath("assets/shaders/");
    auto solid_shader_program = _pCtx->pResManager->Load<voxl::ShaderProgram>(program_info.name, program_info); // cpu side, reste en cache
    _pCtx->pDevice->CreateShaderProgram(solid_shader_program.get()); // gpu side


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

        _pCtx->pDevice->UpdatePipelineShaderProgram(_solidPipelineHandle, shader_to_reload->handle);

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
    attribs[0] = { voxl::AttribLocation::Position, voxl::AttribFormat::Float3, false, 0 };

    voxl::VertexInputLayout input_layout{};
    input_layout.attributes = std::move(attribs);
    input_layout.stride = sizeof(float) * 3; // plus tard ça sera de la taille d'un vertex, pour l'instant position = 3 * float
    input_layout.binding = 0;

    voxl::DepthStencilState depth{};
    depth.enableDepthTest = false;
    depth.enableDepthWrite = false;

    voxl::PipelineDesc pipeline_desc{}; // les autres valeurs sont initialisée par défaut, à voir comment modifier ça si besoin.
    pipeline_desc.program = solid_shader_program->handle;
    pipeline_desc.inputLayout = input_layout;
    pipeline_desc.depthStencil = depth;

    _solidPipelineHandle = _pCtx->pDevice->CreatePipeline(pipeline_desc);


    float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };

    voxl::BufferDesc triangle_desc{};
    triangle_desc.type = voxl::BufferType::Vertex;
    triangle_desc.usage = voxl::BufferUsage::Dynamic;
    triangle_desc.size = sizeof(vertices);
    triangle_desc.data = &vertices[0];

    _triangleVBO = _pCtx->pDevice->CreateBuffer(triangle_desc);
  }


  void OnDetach() override {
    _pCtx->pDevice->DeleteShaderProgram(_pCtx->pResManager->Get<voxl::ShaderProgram>("solid").get());
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
    cmds.Push(voxl::CmdBindVertexBuffer{ _triangleVBO, 0, 0 });
    cmds.Push(voxl::CmdDraw{ 3 });

    _pCtx->pRenderer->SubmitRenderPass(_pCtx->pDevice, cmds);
  }

private:
  voxl::AppContext* _pCtx = nullptr;

  voxl::PipelineHandle _solidPipelineHandle;
  voxl::BufferHandle _triangleVBO;
};


#endif //! SANDBOX_H