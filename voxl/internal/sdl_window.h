#ifndef VOXL_SDL3WINDOW_H
#define VOXL_SDL3WINDOW_H


#include <memory>

#include "window.h"


namespace voxl {


class SDLWindow final: public IWindow {
public:
  explicit SDLWindow(const WindowDesc& desc);
  ~SDLWindow () override;

  void PollEvents() override;
  
  int Width() const override;
  int Height() const override;
  glm::vec2 Extent() const override;
  void* Handle() override;
  bool VSync() const override;

  glm::vec2 Resize(int width, int height) override;

  void SetEventCallback(EventCallback callback) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};


}


#endif //! VOXL_SDL3WINDOW_H