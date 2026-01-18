#include "GLRenderer.h"


#include <imgui.h>
#include <stdexcept>

#include <glad/glad.h>
#include <SDL3/SDL_video.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "window.h"


namespace voxl {


struct GLRenderer::Impl {
  IWindow* pWindow = nullptr;
  SDL_GLContext glContext = nullptr;
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

    ImGui_ImplSDL3_InitForOpenGL(static_cast<SDL_Window*>(_impl->pWindow->Handle()), _impl->glContext);
    ImGui_ImplOpenGL3_Init("#version 460");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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


void GLRenderer::EndFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(static_cast<SDL_Window*>(_impl->pWindow->Handle()));
}


void GLRenderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}


}