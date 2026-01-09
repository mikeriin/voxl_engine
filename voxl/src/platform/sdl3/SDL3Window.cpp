#include "SDL3Window.h"
#include "event.h"


#include <SDL3/SDL_stdinc.h>
#include <utility>
#include <stdexcept>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>


namespace voxl {

struct SDL3Window::Impl {
  SDL_Window* pWindow = nullptr;
  SDL_GLContext glContext = nullptr;
  int width = 0;
  int height = 0;
  EventCallback callback;
};


SDL3Window::SDL3Window(const WindowDesc& desc)
  : _impl(std::make_unique<Impl>())
  {
    if (!SDL_Init(SDL_INIT_VIDEO))
      throw std::runtime_error(SDL_GetError());

    _impl->width = desc.width;
    _impl->height = desc.height;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(0);
    windowFlags |= SDL_WINDOW_RESIZABLE;
    windowFlags |= SDL_WINDOW_OPENGL;

    _impl->pWindow = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, windowFlags);
    if (!_impl->pWindow)
      throw std::runtime_error(SDL_GetError());

    _impl->glContext = SDL_GL_CreateContext(_impl->pWindow);
    if (!_impl->glContext)
      throw std::runtime_error(SDL_GetError());

    SDL_GL_MakeCurrent(_impl->pWindow, _impl->glContext);
    SDL_GL_SetSwapInterval(desc.vsync ? 1: 0);
  }


void SDL3Window::PollEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) {
      WindowCloseEvent event;
      if (_impl->callback) _impl->callback(event);
    }
    else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
      _impl->width = e.window.data1;
      _impl->height = e.window.data2;

      WindowResizeEvent event(_impl->width, _impl->height);
      if (_impl->callback) _impl->callback(event);
    }
  }
}


SDL3Window::~SDL3Window () {
  if (_impl->glContext)
    SDL_GL_DestroyContext(_impl->glContext);
  if (_impl->pWindow)
    SDL_DestroyWindow(_impl->pWindow);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


bool SDL3Window::SwapBuffers() {
  return SDL_GL_SwapWindow(_impl->pWindow);
}


int SDL3Window::Width() const {
  return _impl->width;
}


int SDL3Window::Height() const {
  return _impl->height;
}


glm::vec2 SDL3Window::Extent() const {
  return glm::vec2(
    static_cast<float>(_impl->width),
    static_cast<float>(_impl->height)
  );
}


void SDL3Window::SetEventCallback(EventCallback callback) {
  _impl->callback = std::move(callback);
}

}