#ifndef VOXL_PROGRAM_H
#define VOXL_PROGRAM_H


#include <vector>

#include "res/shader.h"


namespace voxl {


struct ProgramDesc {
  std::vector<Shader*> shaders;
};


class IProgram {
public:
  virtual ~IProgram() = default;
};


}


#endif //! VOXL_PROGRAM_H