#include "gl_render_device.h"
#include "gfx/render_types.h"


#include <glad/glad.h>
#include <print>


namespace voxl {


bool GLRenderDevice::CreateShaderProgram(ShaderProgram* program) {
  if (!program) {
    std::println("The shader program doesn't exist.");
    return false;
  }

  if (program->handle.id == 0) {
    std::println("Failed to create gpu side shader program.");
    return false;
  }

  return true;
}


void GLRenderDevice::DeleteShaderProgram(ShaderProgram* program) {
  glDeleteProgram(program->handle.id);
}


}