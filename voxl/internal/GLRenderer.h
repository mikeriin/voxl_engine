#ifndef VOXL_GLRENDERER_H
#define VOXL_GLRENDERER_H


#include <memory>

#include "renderer.h"


namespace voxl {


class IWindow;


class GLRenderer final: public IRenderer {
public:
  GLRenderer(IWindow* window);
  ~GLRenderer() override;

  void BeginFrame() override;
  void EndFrame() override;

private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
  IWindow* _pWindow;
};


}


#endif //! VOXL_GLRENDERER_H