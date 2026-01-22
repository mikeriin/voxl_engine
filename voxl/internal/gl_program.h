#ifndef VOXL_GL_PROGRAM_H
#define VOXL_GL_PROGRAM_H


#include <cstdint>

#include "gfx/program.h"
#include "res/shader.h"


namespace voxl {


class GLProgram final: public IProgram {
public:
  explicit GLProgram(ProgramDesc& desc);

  uint32_t GetID() const;

  void Delete() const;

private:
  uint32_t _id;

  uint32_t createShader(Shader* shader);

  uint32_t getGLShaderType(ShaderType type);
  inline std::pair<std::string, std::string> splitShaderSource(const std::string& packed);
};


}


#endif //! VOXL_GL_PROGRAM_H