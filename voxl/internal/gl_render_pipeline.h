#ifndef VOXL_GL_RENDER_PIPELINE_H
#define VOXL_GL_RENDER_PIPELINE_H


#include <cstddef>
#include <cstdint>
#include <utility>

#include "gfx/render_pipeline.h"
#include "gfx/render_types.h"


namespace voxl {


class GLRenderPipeline final: public IPipeline {
public:
  explicit GLRenderPipeline(const PipelineDesc& desc);
  ~GLRenderPipeline() override;

  void ApplyState();
  void SetProgram(ProgramHandle handle);

  VertexArrayHandle GetVAO() const;
  ProgramHandle GetProgramHandle() const;
  size_t GetStride() const;
  uint32_t GetTopology() const;

private:
  PipelineDesc _storedDesc;
  ProgramHandle _programHandle;
  VertexArrayHandle _vaoID;
  uint32_t _primitiveTopology;

  uint32_t getGLDepthFunc(CompareOp op);
  uint32_t getGLBlendFactor(BlendFactor factor);
  uint32_t getGLTopology(PrimitiveTopology topo);
  std::pair<int, uint32_t> getGLAttribFormat(AttribFormat format);
};


}


#endif //! VOXL_GL_RENDER_PIPELINE_H