#ifndef VOXL_GL_RENDER_DEVICE_H
#define VOXL_GL_RENDER_DEVICE_H


#include "gfx/render_device.h"
#include "res/shader_program.h"


namespace voxl {


class GLRenderDevice final: public IRenderDevice {
public:
  GLRenderDevice() = default;
  ~GLRenderDevice() = default;

  bool CreateShaderProgram(ShaderProgram* program) override;
  bool ReloadShaderProgram(ShaderProgram* program) override;
  void DeleteShaderProgram(ShaderProgram* program) override;

private:
  unsigned int getShaderType(const ShaderType& type);
  bool compileShader(unsigned int shaderID);
};


}


#endif //! VOXL_GL_RENDER_DEVICE_H