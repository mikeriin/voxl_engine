#include "gl_render_device.h"


#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include <glad/glad.h>

#include "gfx/buffer.h"
#include "gfx/program.h"
#include "gfx/render_pipeline.h"
#include "gl_buffer.h"
#include "gl_program.h"
#include "gl_render_pipeline.h"


namespace voxl {


GLRenderDevice::~GLRenderDevice() {
  _pipelines.clear();

  for (auto& buffer: _buffers) {
    auto gl_buffer = static_cast<GLBuffer*>(buffer.get());
    gl_buffer->Delete();
  }

  for (auto& program: _programs) {
    auto gl_program = static_cast<GLProgram*>(program.get());
    gl_program->Delete();
  }
}


PipelineHandle GLRenderDevice::CreatePipeline(const PipelineDesc& desc) {
  std::unique_ptr<IPipeline> pipeline = std::make_unique<GLRenderPipeline>(desc);
  
  _pipelines.push_back(std::move(pipeline));

  return static_cast<PipelineHandle>(_pipelines.size() - 1);
}


IPipeline* GLRenderDevice::GetPipeline(PipelineHandle handle) {
  assert(handle < _pipelines.size()); // TODO! à enlever en release
  return _pipelines[handle].get();
}


ProgramHandle GLRenderDevice::CreateProgram(ProgramDesc& desc) {
  std::unique_ptr<IProgram> program = std::make_unique<GLProgram>(desc);
  
  _programs.push_back(std::move(program));

  return static_cast<ProgramHandle>(_programs.size() - 1);
}


IProgram* GLRenderDevice::GetProgram(ProgramHandle handle) {
  assert(handle < _programs.size()); // TODO! à enlever en release
  return _programs[handle].get();
}


BufferHandle GLRenderDevice::CreateBuffer(const BufferDesc& desc) {
  std::unique_ptr<IBuffer> buffer = std::make_unique<GLBuffer>(desc);

  _buffers.push_back(std::move(buffer));

  return static_cast<BufferHandle>(_buffers.size() - 1);
}


IBuffer* GLRenderDevice::GetBuffer(BufferHandle handle) {
  assert(handle < _buffers.size()); // TODO! à enlever en release
  return _buffers[handle].get();
}


}