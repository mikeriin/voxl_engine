#ifndef VOXL_GL_BUFFER_H
#define VOXL_GL_BUFFER_H


#include "gfx/buffer.h"
#include <cstdint>


namespace voxl {


class GLBuffer final: public IBuffer {
public:
  explicit GLBuffer(const BufferDesc& desc);

  uint32_t GetID() const;

private:
  uint32_t _id;
};


}


#endif //! VOXL_GL_BUFFER_H