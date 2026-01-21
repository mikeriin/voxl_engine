#include "gl_render_device.h"
#include "gfx/render_pipeline.h"
#include "gl_render_pipeline.h"
#include "res/shader_program.h"


#include <cassert>
#include <cstddef>
#include <glad/glad.h>
#include <memory>
#include <print>
#include <utility>
#include <vector>


namespace voxl {


GLRenderDevice::~GLRenderDevice() {
  _pipelines.clear();
}


bool GLRenderDevice::CreateShaderProgram(ShaderProgram* program) {
  if (!program) {
    std::println("The shader program doesn't exist.");
    return false;
  }

  program->handle = glCreateProgram();
  size_t shader_count = program->shaders.size();
  std::vector<unsigned int> shaders(shader_count);

  for (size_t i = 0; i < shader_count; i++) {
    shaders[i] = glCreateShader(getShaderType(program->shaders[i].type));
    const char* shader_source = program->shaders[i].buffer.c_str();
    glShaderSource(shaders[i], 1, &shader_source, nullptr);

    if (!compileShader(shaders[i])) {
      glDeleteShader(shaders[i]);
      shaders[i] = 0;
    }
    else glAttachShader(program->handle, shaders[i]);
  }

  glLinkProgram(program->handle);

  int success = 0;
  glGetProgramiv(program->handle, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(program->handle, 512, nullptr, info_log);
    std::println("Failed to link program : {}", info_log);

    for (auto& shader : shaders) {
      glDetachShader(program->handle, shader);
      glDeleteShader(shader);
    }

    glDeleteProgram(program->handle);

    std::println("Failed to create gpu side shader program.");
    return false;
  }

  for (size_t i = 0; i < shader_count; i++) {
    if (shaders[i] != 0) {
      glDetachShader(program->handle, shaders[i]);
      glDeleteShader(shaders[i]);
    }
    program->shaders[i].buffer.clear();
  }

  return true;
}


bool GLRenderDevice::ReloadShaderProgram(ShaderProgram* program) {
  if (!program) {
    std::println("The shader program doesn't exist.");
    return false;
  }

  unsigned int new_program = glCreateProgram();
  size_t shader_count = program->shaders.size();
  std::vector<unsigned int> shaders(shader_count);

  for (size_t i = 0; i < shader_count; i++) {
    shaders[i] = glCreateShader(getShaderType(program->shaders[i].type));
    const char* shader_source = program->shaders[i].buffer.c_str();
    glShaderSource(shaders[i], 1, &shader_source, nullptr);

    if (!compileShader(shaders[i])) {
      glDeleteShader(shaders[i]);
      shaders[i] = 0;

      glDeleteProgram(new_program);
      return false;
    }
    else glAttachShader(new_program, shaders[i]);
  }

  glLinkProgram(new_program);

  int success = 0;
  glGetProgramiv(new_program, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(new_program, 512, nullptr, info_log);
    std::println("Failed to link program : {}", info_log);

    for (auto& shader : shaders) {
      glDetachShader(new_program, shader);
      glDeleteShader(shader);
    }

    glDeleteProgram(new_program);

    std::println("Failed to create gpu side shader program.");
    return false;
  }

  for (size_t i = 0; i < shader_count; i++) {
    if (shaders[i] != 0) {
      glDetachShader(new_program, shaders[i]);
      glDeleteShader(shaders[i]);
    }
    program->shaders[i].buffer.clear();
  }

  program->handle = new_program;
  glDeleteProgram(program->handle);

  return true;
}


void GLRenderDevice::DeleteShaderProgram(ShaderProgram* program) {
  glDeleteProgram(program->handle);
}


PipelineHandle GLRenderDevice::CreatePipeline(const PipelineDesc& desc) {
  std::unique_ptr<IPipeline> pipeline = std::make_unique<GLRenderPipeline>(desc);
  
  _pipelines.push_back(std::move(pipeline));

  return static_cast<PipelineHandle>(_pipelines.size() - 1);
}


IPipeline* GLRenderDevice::GetPipeline(PipelineHandle handle) {
  assert(handle < _pipelines.size()); // TODO! à enlever en release
  return _pipelines[handle].get();
}


unsigned int GLRenderDevice::getShaderType(const ShaderType& type) {
  switch (type) {
  case ShaderType::VERTEX:
    return GL_VERTEX_SHADER;

  case ShaderType::FRAGMENT:
    return GL_FRAGMENT_SHADER;

  default:
    return 0;
  }
}


bool GLRenderDevice::compileShader(unsigned int shaderID) {
  glCompileShader(shaderID);

  int success = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(shaderID, 512, nullptr, info_log);
    std::println("Failed to compile shader : {}", info_log);
    return false;
  }

  return true;
}


}