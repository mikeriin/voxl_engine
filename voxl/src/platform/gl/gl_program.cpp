#include "gl_program.h"


#include <cstdint>

#include <glad/glad.h>
#include <string_view>

#include "res/shader.h"


namespace voxl {


GLProgram::GLProgram(ProgramDesc& desc) {
  _id = glCreateProgram();

  std::vector<uint32_t> shaders;

  for (auto shader_cpu : desc.shaders) shaders.push_back(createShader(shader_cpu));

  glLinkProgram(_id);

  int success = 0;
  glGetProgramiv(_id, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(_id, 512, nullptr, info_log);
    std::println("Failed to link program : {}", info_log);
    
    for (auto shader_gpu: shaders) {
      if (shader_gpu != 0) {
        glDetachShader(_id, shader_gpu);
        glDeleteShader(shader_gpu);
      }
    }

    glDeleteProgram(_id);
  }

  for (auto shader_gpu: shaders) {
    glDetachShader(_id, shader_gpu);
    glDeleteShader(shader_gpu);
  }
}


uint32_t GLProgram::GetID() const {
  return _id;
}


void GLProgram::Delete() const {
  glDeleteProgram(_id);
}


uint32_t GLProgram::createShader(Shader* shader) {
  uint32_t out = glCreateShader(getGLShaderType(shader->type));

  auto [path, buffer] = splitShaderSource(shader->source);
  const char* source = buffer.c_str();
  glShaderSource(out, 1, &source, nullptr);
  glCompileShader(out);

  int success = 0;
  glGetShaderiv(out, GL_COMPILE_STATUS, &success);

  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(out, 512, nullptr, info_log);
    std::println("Failed to compile shader : {}", info_log);
    glDeleteShader(out);
    return 0;
  }

  glAttachShader(_id, out);

  shader->source = path;

  return out;
}


uint32_t GLProgram::getGLShaderType(ShaderType type) {
  switch (type) {
  case ShaderType::Vertex:
    return GL_VERTEX_SHADER;

  case ShaderType::Fragment:
    return GL_FRAGMENT_SHADER;
  }
}


inline std::pair<std::string, std::string> GLProgram::splitShaderSource(const std::string& packed) {
  const std::string_view separator = SOURCE_SEPARATOR;

  const auto pos = packed.find(separator);
  if (pos == std::string::npos) {
    return {"", packed};
  }

  std::string path = packed.substr(0, pos);
  std::string source = packed.substr(pos + separator.size());

  return {std::move(path), std::move(source)};
}


}