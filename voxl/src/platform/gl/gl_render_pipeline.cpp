#include "gl_render_pipeline.h"


#include <glad/glad.h>

#include "gfx/render_pipeline.h"


namespace voxl {


GLRenderPipeline::GLRenderPipeline(const PipelineDesc& desc) {
  _storedDesc = desc;
}


void GLRenderPipeline::ApplyState() {
  glUseProgram(_storedDesc.program);

  if (_storedDesc.depthStencil.enableDepthTest) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);

  glDepthFunc(getGLDepthFunc(_storedDesc.depthStencil.depthCompareOp));

  if (_storedDesc.rasterizer.cullMode == CullMode::None) glDisable(GL_CULL_FACE);
  else {
    glEnable(GL_CULL_FACE);
    glCullFace(_storedDesc.rasterizer.cullMode == CullMode::Front ? GL_FRONT: GL_BACK);
  }

  if (_storedDesc.blend.enableBlend) {
    glEnable(GL_BLEND);
    glBlendFunc(getGLBlendFactor(_storedDesc.blend.srcFactor), getGLBlendFactor(_storedDesc.blend.dstFactor));
  }
  else glDisable(GL_BLEND);

  glPolygonMode(GL_FRONT_AND_BACK, _storedDesc.rasterizer.polygonMode == PolygonMode::Fill ? GL_FILL: GL_LINE);
}


uint32_t GLRenderPipeline::getGLDepthFunc(CompareOp op) {
  switch (op) {
    case CompareOp::Less:
      return GL_LESS;

    case CompareOp::Greater:
      return GL_GREATER;

    case CompareOp::Always:
      return GL_ALWAYS;
  }
}


uint32_t GLRenderPipeline::getGLBlendFactor(BlendFactor factor) {
  switch (factor) {
    case BlendFactor::Src:
      return GL_SRC_ALPHA;

    case BlendFactor::OneMinusSrc:
      return GL_ONE_MINUS_SRC_ALPHA;

    case BlendFactor::Dst:
      return GL_DST_ALPHA;

    case BlendFactor::OneMinusDst:
      return GL_ONE_MINUS_DST_ALPHA;
  }
}


}