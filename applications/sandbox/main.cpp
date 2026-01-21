#include <voxl.h>

#include "sandbox.h"


int main(int argc, char* argv[]) 
{
  voxl::Application app(voxl::WindowDesc{}, voxl::GraphicsAPI::OpenGL);
  app.PushOverlay(std::make_unique<voxl::DevConsole>());
  app.PushLayer(std::make_unique<SandboxLayer>());
  app.Run();

  return 0;
}