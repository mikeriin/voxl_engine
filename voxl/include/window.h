#ifndef VOXL_WINDOW_H
#define VOXL_WINDOW_H


#include <string>
#include <functional>

#include <glm/glm.hpp>

#include "event.h"


namespace voxl {


struct WindowDesc {
  std::string title = "voxl";
  int width = 1280;
  int height = 720;
  bool vsync = true;
};

class IWindow {
public:
  using EventCallback = std::function<void(Event&)>;

  virtual ~IWindow() = default;

  virtual void PollEvents() = 0;

  virtual int Width() const = 0;
  virtual int Height() const = 0;
  virtual glm::vec2 Extent() const = 0;
  virtual void* Handle() = 0;
  virtual bool VSync() const = 0;

  virtual glm::vec2 Resize(int width, int height) = 0;

  virtual void SetEventCallback(EventCallback callback) = 0;
};

}


#endif //! VOXL_WINDOW_H