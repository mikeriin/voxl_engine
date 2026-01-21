#ifndef VOXL_RENDER_COMMAND_H
#define VOXL_RENDER_COMMAND_H


#include "render_types.h"
#include <glm/gtc/matrix_transform.hpp>
#include <variant>
#include <vector>


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


using RenderCommand = std::variant<
  CmdSetViewport,
  CmdBindPipeline
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