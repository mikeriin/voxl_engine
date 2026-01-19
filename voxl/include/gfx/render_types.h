#ifndef VOXL_RENDER_TYPES_H
#define VOXL_RENDER_TYPES_H


#include <cstdint>

#include <glm/glm.hpp>


namespace voxl {


struct Handle { uint32_t id = 0; };


struct BufferHandle: public Handle {};


struct TextureHandle: public Handle {};


struct MeshHandle: public Handle {};


struct ShaderHandle: public Handle {};
struct ShaderProgramHandle: public Handle {};


struct MaterialHandle: public Handle {
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