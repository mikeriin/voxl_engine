#ifndef VOXL_GL_RENDER_DEVICE_H
#define VOXL_GL_RENDER_DEVICE_H


#include "gfx/render_device.h"


namespace voxl {


class GLRenderDevice final: public IRenderDevice {
public:
  ~GLRenderDevice();

  PipelineHandle CreatePipeline(const PipelineDesc& desc) override;
  IPipeline* GetPipeline(PipelineHandle handle) override;

  ProgramHandle CreateProgram(ProgramDesc& desc) override;
  IProgram* GetProgram(ProgramHandle handle) override;

  BufferHandle CreateBuffer(const BufferDesc& desc) override;
  IBuffer* GetBuffer(BufferHandle handle) override;
};


}


#endif //! VOXL_GL_RENDER_DEVICE_H