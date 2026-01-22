#include "gl_buffer.h"
#include "gfx/buffer.h"


#include <glad/glad.h>


namespace voxl {


GLBuffer::GLBuffer(const BufferDesc& desc) {
  glCreateBuffers(1, &_id);

  switch (desc.usage) {
  case BufferUsage::Immutable:
    break;

  case BufferUsage::Dynamic: {
    glNamedBufferData(_id, desc.size, desc.data, GL_DYNAMIC_DRAW);
  } break;

  case BufferUsage::Stream:
    break;
  }
}


uint32_t GLBuffer::GetID() const {
  return _id;
}


void GLBuffer::Delete() const {
  glDeleteBuffers(1, &_id);
}


}