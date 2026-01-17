#ifndef VOXL_SHADER_PROGRAM_H
#define VOXL_SHADER_PROGRAM_H


#include <print>
#include <string>

#include "resource.h"
#include "resource_loader.h"


namespace voxl {


struct ShaderProgram final: public Resource { };

template<>
struct ResourceLoader<ShaderProgram> {
  static ShaderProgram Load(const std::string& name) {
    ShaderProgram program{};
    

    std::println("Successfully created shader program.");
    return program;
  }
};


}


#endif //! VOXL_SHADER_PROGRAM_H