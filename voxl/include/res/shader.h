#ifndef VOXL_SHADER_H
#define VOXL_SHADER_H


#include <print>
#include <string>

#include "resource.h"
#include "utils.h"


namespace voxl {


enum class ShaderType {
  Vertex,
  Fragment
};


struct Shader final: Resource {
  ShaderType type;
  std::string source; // le chemin du shader + son contenu, contenu supprimé après création, maintient du chemin pour reload
};


struct ShaderDesc {
  ShaderType type;
  std::string path;
};


inline constexpr const char* SOURCE_SEPARATOR = "$ù$ù$";


template<>
struct Loader<Shader> {
  static std::shared_ptr<Shader> Load(const ShaderDesc& desc) {
    auto shader = std::make_shared<Shader>();

    std::string path = _relativePath + desc.path;
    auto src = ReadFile(path);
    if (src.empty()) {
      std::println("{} is empty", path);
      return nullptr;
    }

    shader->type = desc.type;
    shader->source = desc.path + SOURCE_SEPARATOR + src;

    return shader;
  }

  static void SetRelativePath(const std::string& relative) {
    _relativePath = relative;
  }

private:
  static std::string _relativePath;
};

}


#endif //! VOXL_SHADER_H