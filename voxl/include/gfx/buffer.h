#ifndef VOXL_BUFFER_H
#define VOXL_BUFFER_H


#include <cstdint>
namespace voxl {


enum class BufferType {
  Vertex,
  Index,
  Uniform, // ubo
  Storage // ssbo
};


enum class BufferUsage {
  Immutable, // NamedBufferStorage, utile pour un ubo ou ssbo, MapNamedBufferRange pour modifier la mémoire, un peu plus complexe
  Dynamic,  // NamedBufferData, NamedBufferSubData pour réallouer la mémoire, simple
  Stream
};


struct BufferDesc {
  BufferType type;
  BufferUsage usage;
  uint32_t size; // taille en octets
  const void* data; // peut être nullptr
};


class IBuffer {
public:
  virtual ~IBuffer() = default;
};


}


#endif //! VOXL_BUFFER_H