#ifndef VOXL_RENDER_DEVICE_H
#define VOXL_RENDER_DEVICE_H


#include <vector>
#include <memory>

#include "gfx/render_pipeline.h"
#include "gfx/render_types.h"
#include "render_types.h"
#include "res/shader_program.h"


namespace voxl {


class IRenderDevice {
public: 
  virtual ~IRenderDevice() = default;

  virtual bool CreateShaderProgram(ShaderProgram* program) = 0;
  virtual bool ReloadShaderProgram(ShaderProgram* program) = 0;
  virtual void DeleteShaderProgram(ShaderProgram* program) = 0;

  virtual PipelineHandle CreatePipeline(const PipelineDesc& desc) = 0;
  virtual IPipeline* GetPipeline(PipelineHandle handle) = 0;

protected:
  std::vector<std::unique_ptr<IPipeline>> _pipelines;
};


}


#endif //! VOXL_RENDER_DEVICE_H