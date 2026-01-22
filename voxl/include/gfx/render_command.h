#ifndef VOXL_RENDER_COMMAND_H
#define VOXL_RENDER_COMMAND_H


#include <cstdint>
#include <variant>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "render_types.h"


namespace voxl {


struct CmdSetViewport { 
  float x;
  float y;
  float w;
  float h; 
};


struct CmdBindPipeline {
  PipelineHandle handle;
};


struct CmdBindVertexBuffer {
  BufferHandle handle;
  uint32_t binding = 0;
  int offset = 0;
};


struct CmdBindIndexBuffer {
  BufferHandle handle;
};


struct CmdDraw {
  uint32_t vertexCount;
};


struct CmdDrawElements {
  uint32_t indexCount;
};


using RenderCommand = std::variant<
  CmdSetViewport,
  CmdBindPipeline,
  CmdBindVertexBuffer,
  CmdBindIndexBuffer,
  CmdDraw,
  CmdDrawElements
>;


class CommandBuffer {
public:
  void Push(RenderCommand cmd);
  std::vector<RenderCommand>& GetCommands();
  void Clear();

private:
  std::vector<RenderCommand> _commands;
};  


}


#endif //! VOXL_RENDER_COMMAND_H