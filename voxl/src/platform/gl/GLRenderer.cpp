#include "GLRenderer.h"


#include <stdexcept>

#include <glad/glad.h>
#include <SDL3/SDL_video.h>

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
  }


GLRenderer::~GLRenderer() {
  if (_impl->glContext)
    SDL_GL_DestroyContext(_impl->glContext);
}


void GLRenderer::BeginFrame() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


void GLRenderer::EndFrame() {
  SDL_GL_SwapWindow(static_cast<SDL_Window*>(_impl->pWindow->Handle()));
}


}