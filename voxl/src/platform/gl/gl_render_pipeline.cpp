#include "gl_render_pipeline.h"


#include <cstdint>
#include <glad/glad.h>
#include <utility>

#include "gfx/render_pipeline.h"


namespace voxl {


GLRenderPipeline::GLRenderPipeline(const PipelineDesc& desc) {
  _storedDesc = desc;
  
  _primitiveTopology = getGLTopology(_storedDesc.topology);
  _programHandle = desc.program;

  glCreateVertexArrays(1, &_vaoID);

  for (const auto& attr: desc.inputLayout.attributes) {
    glEnableVertexArrayAttrib(_vaoID, attr.location);

    auto attr_format = getGLAttribFormat(attr.format);
    uint32_t normalized_gl = attr.normalized ? GL_TRUE: GL_FALSE;

    if (attr_format.second == GL_FLOAT)
      glVertexArrayAttribFormat(_vaoID, attr.location, attr_format.first, GL_FLOAT, normalized_gl, attr.offset);
    else if (attr_format.second == GL_UNSIGNED_INT)
      glVertexArrayAttribIFormat(_vaoID, attr.location, attr_format.first, GL_UNSIGNED_INT, attr.offset);

    glVertexArrayAttribBinding(_vaoID, attr.location, desc.inputLayout.binding);
  }
}


GLRenderPipeline::~GLRenderPipeline() {
  if (_vaoID)
    glDeleteVertexArrays(1, &_vaoID);
}


void GLRenderPipeline::ApplyState() {
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


void GLRenderPipeline::SetProgram(ProgramHandle handle) {
  _storedDesc.program = handle;
}


VertexArrayHandle GLRenderPipeline::GetVAO() const {
  return _vaoID;
}


ProgramHandle GLRenderPipeline::GetProgramHandle() const {
  return _programHandle;
}


size_t GLRenderPipeline::GetStride() const {
  return _storedDesc.inputLayout.stride;
}


uint32_t GLRenderPipeline::GetTopology() const {
  return _primitiveTopology;
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


uint32_t GLRenderPipeline::getGLTopology(PrimitiveTopology topo) {
  switch (topo) {
    case PrimitiveTopology::Triangle:
      return GL_TRIANGLES;
    
    case PrimitiveTopology::Line:
      return GL_LINE;

    case PrimitiveTopology::Point:
      return GL_POINT;
  }
}


std::pair<int, uint32_t> GLRenderPipeline::getGLAttribFormat(AttribFormat format) {
  switch (format) {
    case AttribFormat::Float3:
      return std::make_pair(3, GL_FLOAT);

    case AttribFormat::UInt1:
      return std::make_pair(1, GL_UNSIGNED_INT);

    default:
      return std::make_pair(0, 0);
  }
}


}