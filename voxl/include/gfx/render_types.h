#ifndef VOXL_RENDER_TYPES_H
#define VOXL_RENDER_TYPES_H


#include <cstdint>

#include <glm/glm.hpp>


namespace voxl {


struct GpuHandle { uint32_t id = 0; };


struct BufferHandle: public GpuHandle {};


struct TextureHandle: public GpuHandle {};


struct MeshHandle: public GpuHandle {};


struct ShaderProgramHandle: public GpuHandle {};


struct MaterialHandle: public GpuHandle {
  ShaderProgramHandle shader;

  TextureHandle albedoMap;
  TextureHandle normalMap;
  TextureHandle roughnessMap;

  glm::vec4 baseColor;
  float roughness;
  float metalic;
};


enum class BufferType {
  VERTEX,
  INDEX,
  UNIFORM,
  STORAGE,
};


}


#endif //! VOXL_RENDER_TYPES_H