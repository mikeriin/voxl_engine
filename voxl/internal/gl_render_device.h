#ifndef VOXL_GL_RENDER_DEVICE_H
#define VOXL_GL_RENDER_DEVICE_H


#include "gfx/render_device.h"


namespace voxl {


class GLRenderDevice final: public IRenderDevice {
public:
  GLRenderDevice() = default;
  ~GLRenderDevice() = default;

  bool CreateShaderProgram(ShaderProgram* program) override;
  void DeleteShaderProgram(ShaderProgram* program) override;
};


}


#endif //! VOXL_GL_RENDER_DEVICE_H