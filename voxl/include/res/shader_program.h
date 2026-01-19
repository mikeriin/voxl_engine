#ifndef VOXL_SHADER_PROGRAM_H
#define VOXL_SHADER_PROGRAM_H


#include <cstddef>
#include <print>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "resource.h"
#include "gfx/render_types.h"
#include "utils.h"


namespace voxl {


enum class ShaderType {
  VERTEX,
  FRAGMENT,
};


struct ShaderCreateInfo {
  ShaderType type;
  std::string path;
};


struct ShaderProgramCreateInfo {
  std::string name;
  std::span<const ShaderCreateInfo> shaders;
};


struct Shader {
  ShaderHandle handle;
  ShaderType type;
  std::string buffer;
};


struct ShaderProgram final: Resource {
  ShaderProgramHandle handle;
  std::vector<Shader> shaders;
};


template<>
struct Loader<ShaderProgram> {
  static std::shared_ptr<ShaderProgram> Load(const ShaderProgramCreateInfo& createInfo) {
    auto program = std::make_shared<ShaderProgram>();
    
    size_t shader_count = createInfo.shaders.size();
    program->shaders.resize(shader_count);

    for (size_t i = 0; i < shader_count; i++) {
      const auto& shaderInfo = createInfo.shaders[i];

      program->shaders[i].type = shaderInfo.type;

      auto src = ReadFile(shaderInfo.path);
      if (src.empty()) {
        std::println("{} is empty", shaderInfo.path);
        return nullptr;
      }
      program->shaders[i].buffer = std::move(src);
    }

    std::println("Successfully created cpu side shader program");

    return program;
  }
};


}


#endif //! VOXL_SHADER_PROGRAM_H