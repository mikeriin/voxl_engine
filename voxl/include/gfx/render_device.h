#ifndef VOXL_RENDER_DEVICE_H
#define VOXL_RENDER_DEVICE_H


#include "res/shader_program.h"


namespace voxl {


class IRenderDevice {
public: 
  virtual ~IRenderDevice() = default;

  virtual bool CreateShaderProgram(ShaderProgram* program) = 0;
  virtual void DeleteShaderProgram(ShaderProgram* program) = 0;
};


}


#endif //! VOXL_RENDER_DEVICE_H