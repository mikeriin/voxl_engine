#include "gl_renderer.h"


#include <memory>
#include <stdexcept>
#include <variant>
#include <iostream>

#include <glad/glad.h>
#include <SDL3/SDL_video.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

#include "gfx/buffer.h"
#include "gfx/render_pipeline.h"
#include "gl_buffer.h"
#include "gl_render_pipeline.h"
#include "window.h"
#include "gfx/render_device.h"
#include "gfx/render_command.h"


static void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
  std::cerr << "[GL] type=0x" << std::hex << type << " severity=0x" << severity << " id=" << std::dec << id << " msg=" << message << "\n";
}


namespace voxl {


struct GLRenderer::Impl {
  IWindow* pWindow = nullptr;
  SDL_GLContext glContext = nullptr;
  GLRenderPipeline* pCurrentPipeline = nullptr;
};


GLRenderer::GLRenderer(IWindow* window)
  : _impl(std::make_unique<Impl>())
  {
    _impl->pWindow = window;

    _impl->glContext = SDL_GL_CreateContext(static_cast<SDL_Window*>(_impl->pWindow->Handle()));
    if (!_impl->glContext)
      throw std::runtime_error(SDL_GetError());

    if (!SDL_GL_MakeCurrent(static_cast<SDL_Window*>(_impl->pWindow->Handle()), _impl->glContext))
      throw std::runtime_error(SDL_GetError());

    SDL_GL_SetSwapInterval(_impl->pWindow->VSync() ? 1: 0);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
      throw std::runtime_error("Failed to load GL.\n");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(_impl->pWindow->Handle()), _impl->glContext);
    ImGui_ImplOpenGL3_Init("#version 460");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glViewport(0, 0, _impl->pWindow->Width(), _impl->pWindow->Height());
  }


GLRenderer::~GLRenderer() {
  if (_impl->glContext)
    SDL_GL_DestroyContext(_impl->glContext);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
}


void GLRenderer::BeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  glClear(GL_COLOR_BUFFER_BIT);
}


void GLRenderer::SubmitRenderPass(IRenderDevice* device, CommandBuffer& cmds) {
  for (const auto& cmd : cmds.GetCommands()) {
    if (auto* data = std::get_if<CmdBindPipeline>(&cmd)) {
      IPipeline* pipeline = device->GetPipeline(data->handle);
      auto* glPipeline = static_cast<GLRenderPipeline*>(pipeline);
      glPipeline->ApplyState();

      _impl->pCurrentPipeline = glPipeline;

      glBindVertexArray(_impl->pCurrentPipeline->GetVAO());
    }
    else if (auto* data = std::get_if<CmdBindVertexBuffer>(&cmd)) {
      IBuffer* buffer = device->GetBuffer(data->handle);
      auto* glBuffer = static_cast<GLBuffer*>(buffer);

      glVertexArrayVertexBuffer(
        _impl->pCurrentPipeline->GetVAO(), 
        data->binding, 
        glBuffer->GetID(), 
        data->offset, 
        _impl->pCurrentPipeline->GetStride()
      );
    }
    else if (auto* data = std::get_if<CmdBindIndexBuffer>(&cmd)) {
      IBuffer* buffer = device->GetBuffer(data->handle);
      auto* glBuffer = static_cast<GLBuffer*>(buffer);

      glVertexArrayElementBuffer(_impl->pCurrentPipeline->GetVAO(), glBuffer->GetID());
    }
    else if (auto* data = std::get_if<CmdDraw>(&cmd)) {
      glDrawArrays(_impl->pCurrentPipeline->GetTopology(), 0, data->vertexCount);
    }
    else if (auto* data = std::get_if<CmdDrawElements>(&cmd)) {
      glDrawElements(_impl->pCurrentPipeline->GetTopology(), data->indexCount, GL_UNSIGNED_INT, nullptr);
    }
  }

  cmds.Clear();
}


void GLRenderer::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(static_cast<SDL_Window*>(_impl->pWindow->Handle()));
}


}