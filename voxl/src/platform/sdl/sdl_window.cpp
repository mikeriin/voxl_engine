#include "sdl_window.h"


#include <cstdint>
#include <imgui_impl_sdl3.h>
#include <utility>
#include <stdexcept>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <imgui.h>


namespace voxl {

struct SDLWindow::Impl {
  SDL_Window* pWindow = nullptr;
  int width = 0;
  int height = 0;
  bool vsync = false;
  EventCallback callback;
};


SDLWindow::SDLWindow(const WindowDesc& desc)
  : _impl(std::make_unique<Impl>())
  {
    if (!SDL_Init(SDL_INIT_VIDEO))
      throw std::runtime_error(SDL_GetError());

    _impl->width = desc.width;
    _impl->height = desc.height;
    _impl->vsync = desc.vsync;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // TODO! à enlever pour la release
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(0);
    window_flags |= SDL_WINDOW_RESIZABLE;
    window_flags |= SDL_WINDOW_OPENGL;

    _impl->pWindow = SDL_CreateWindow(desc.title.c_str(), desc.width, desc.height, window_flags);
    if (!_impl->pWindow)
      throw std::runtime_error(SDL_GetError());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
  }


void SDLWindow::PollEvents() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    ImGui_ImplSDL3_ProcessEvent(&e);

    switch (e.type) {
    case SDL_EVENT_QUIT: {
      WindowCloseEvent event;
      if (_impl->callback) _impl->callback(event);
    } break;

    case SDL_EVENT_WINDOW_RESIZED: {
      _impl->width = e.window.data1;
      _impl->height = e.window.data2;

      WindowResizeEvent event(_impl->width, _impl->height);
      if (_impl->callback) _impl->callback(event);
    } break;

    case SDL_EVENT_KEY_DOWN: {
      KeyPressedEvent event(
        static_cast<int>(e.key.scancode),
        static_cast<int>(e.key.key),
        static_cast<uint16_t>(e.key.mod),
        e.key.repeat
      );

      if (_impl->callback) _impl->callback(event);
    } break;

    case SDL_EVENT_KEY_UP: {
      KeyReleasedEvent event(
        static_cast<int>(e.key.scancode),
        static_cast<int>(e.key.key),
        static_cast<uint16_t>(e.key.mod)
      );
      
      if (_impl->callback) _impl->callback(event);
    } break;

    default: break;
    }
  }
}


SDLWindow::~SDLWindow () {
  ImGui::DestroyContext();

  if (_impl->pWindow)
    SDL_DestroyWindow(_impl->pWindow);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


int SDLWindow::Width() const {
  return _impl->width;
}


int SDLWindow::Height() const {
  return _impl->height;
}


glm::vec2 SDLWindow::Extent() const {
  return glm::vec2(
    static_cast<float>(_impl->width),
    static_cast<float>(_impl->height)
  );
}


void* SDLWindow::Handle() {
  return _impl->pWindow;
}


bool SDLWindow::VSync() const {
  return _impl->vsync;
}


glm::vec2 SDLWindow::Resize(int width, int height) {
  SDL_SetWindowSize(_impl->pWindow, width, height);
  return glm::vec2(
    static_cast<float>(width),
    static_cast<float>(height)
  );
}


void SDLWindow::SetEventCallback(EventCallback callback) {
  _impl->callback = std::move(callback);
}

}