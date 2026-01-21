#include "gfx/render_command.h"


namespace voxl {


void CommandBuffer::Push(RenderCommand cmd) {
  _commands.push_back(cmd);
}


std::vector<RenderCommand>& CommandBuffer::GetCommands() {
  return _commands;
}


void CommandBuffer::Clear() {
  _commands.clear();
}


}