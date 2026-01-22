#ifndef VOXL_GL_RENDER_DEVICE_H
#define VOXL_GL_RENDER_DEVICE_H


#include "gfx/render_device.h"
#include "gfx/render_pipeline.h"
#include "res/shader_program.h"


namespace voxl {


class GLRenderDevice final: public IRenderDevice {
public:
  ~GLRenderDevice();

  bool CreateShaderProgram(ShaderProgram* program) override;
  bool ReloadShaderProgram(ShaderProgram* program) override;
  void DeleteShaderProgram(ShaderProgram* program) override;

  PipelineHandle CreatePipeline(const PipelineDesc& desc) override;
  void UpdatePipelineShaderProgram(PipelineHandle handle, ShaderProgramHandle program) override;
  IPipeline* GetPipeline(PipelineHandle handle) override;

  BufferHandle CreateBuffer(const BufferDesc& desc) override;
  IBuffer* GetBuffer(BufferHandle handle) override;

private:
  unsigned int getShaderType(const ShaderType& type);
  bool compileShader(unsigned int shaderID);
};


}


#endif //! VOXL_GL_RENDER_DEVICE_H