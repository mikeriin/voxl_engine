#ifndef VOXL_RENDER_DEVICE_H
#define VOXL_RENDER_DEVICE_H


#include <vector>
#include <memory>

#include "gfx/buffer.h"
#include "gfx/program.h"
#include "gfx/render_pipeline.h"
#include "gfx/render_types.h"


namespace voxl {


class IRenderDevice {
public: 
  virtual ~IRenderDevice() = default;

  virtual PipelineHandle CreatePipeline(const PipelineDesc& desc) = 0;
  virtual IPipeline* GetPipeline(PipelineHandle handle) = 0;

  virtual ProgramHandle CreateProgram(ProgramDesc& desc) = 0;
  virtual IProgram* GetProgram(ProgramHandle handle) = 0;

  virtual BufferHandle CreateBuffer(const BufferDesc& desc) = 0;
  virtual IBuffer* GetBuffer(BufferHandle handle) = 0;

protected:
  std::vector<std::unique_ptr<IPipeline>> _pipelines;
  std::vector<std::unique_ptr<IProgram>> _programs;
  std::vector<std::unique_ptr<IBuffer>> _buffers;
};


}


#endif //! VOXL_RENDER_DEVICE_H