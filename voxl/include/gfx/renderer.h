#ifndef VOXL_RENDERER_H
#define VOXL_RENDERER_H


namespace voxl {


class IRenderDevice;


class IRenderer {
public:
  virtual ~IRenderer() = default;

  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;

  virtual IRenderDevice* Device() = 0;
};


}


#endif //! VOXL_RENDERER_H