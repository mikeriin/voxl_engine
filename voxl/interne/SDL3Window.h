#ifndef VOXL_SDL3WINDOW_H
#define VOXL_SDL3WINDOW_H


#include <memory>

#include "window.h"


namespace voxl {

class SDL3Window final: public IWindow {
public:
  explicit SDL3Window(const WindowDesc& desc);
  ~SDL3Window () override;

  void PollEvents() override;
  bool SwapBuffers() override;
  
  int Width() const override;
  int Height() const override;
  glm::vec2 Extent() const override;

  void SetEventCallback(EventCallback callback) override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

}


#endif //! VOXL_SDL3WINDOW_H