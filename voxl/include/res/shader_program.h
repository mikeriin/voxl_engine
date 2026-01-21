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
  ShaderType type;
  std::string path;
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
    program->shaders.resize(shader_count, Shader{});

    for (size_t i = 0; i < shader_count; i++) {
      const auto& shaderInfo = createInfo.shaders[i];

      program->shaders[i].type = shaderInfo.type;
      program->shaders[i].path = shaderInfo.path;

      std::string path = _relativePath + shaderInfo.path;
      auto src = ReadFile(path);
      if (src.empty()) {
        std::println("{} is empty", path);
        return nullptr;
      }
      program->shaders[i].buffer = std::move(src);
    }

    _shaderList.push_back(createInfo.name);

    return program;
  }

  
  static bool Reload(ShaderProgram* program) {
    size_t shader_count = program->shaders.size();

    for (size_t i = 0; i < shader_count; i++) {
      std::string path = _relativePath + program->shaders[i].path;

      auto src = ReadFile(path);
      if (src.empty()) {
        std::println("{} is empty", path);
        return false;
      }
      program->shaders[i].buffer = std::move(src);
    }

    return true;
  }


  static void DefineRelativePath(const std::string& relative) {
    _relativePath = relative;
  }

  
  static std::string GetShaderList() {
    std::string buffer;

    for (const auto& name : _shaderList) 
      buffer += name + "\n";

    buffer.resize(buffer.size() - 1);

    return buffer;
  }

private:
  static std::string _relativePath;
  static std::vector<std::string> _shaderList;
};


}


#endif //! VOXL_SHADER_PROGRAM_H