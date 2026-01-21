#ifndef VOXL_GL_RENDER_PIPELINE_H
#define VOXL_GL_RENDER_PIPELINE_H


#include <cstdint>

#include "gfx/render_pipeline.h"


namespace voxl {


class GLRenderPipeline final: public IPipeline {
public:
  explicit GLRenderPipeline(const PipelineDesc& desc);

  void ApplyState();

private:
  PipelineDesc _storedDesc;

  uint32_t getGLDepthFunc(CompareOp op);
  uint32_t getGLBlendFactor(BlendFactor factor);
};


}


#endif //! VOXL_GL_RENDER_PIPELINE_H