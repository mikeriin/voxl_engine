#ifndef VOXL_GLRENDERER_H
#define VOXL_GLRENDERER_H


#include <memory>

#include "gfx/renderer.h"


namespace voxl {


class IWindow;
class IRenderDevice;


class GLRenderer final: public IRenderer {
public:
  GLRenderer(IWindow* window);
  ~GLRenderer() override;

  void BeginFrame() override;
  void EndFrame() override;

  IRenderDevice* Device() override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};


}


#endif //! VOXL_GLRENDERER_H