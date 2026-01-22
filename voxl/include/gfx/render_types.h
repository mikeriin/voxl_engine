#ifndef VOXL_RENDER_TYPES_H
#define VOXL_RENDER_TYPES_H


#include <cstdint>

#include <glm/glm.hpp>


namespace voxl {


enum class GraphicsAPI {
  OpenGL,
  Vulkan,
  DirectX12
};

using ShaderProgramHandle = uint32_t;
using RenderPassHandle = uint32_t;
using PipelineHandle = uint32_t;
using BufferHandle = uint32_t;
using VertexArrayHandle = uint32_t;


}


#endif //! VOXL_RENDER_TYPES_H