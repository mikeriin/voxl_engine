#include <voxl.h>

#include "sandbox.h"


int main(int argc, char* argv[]) 
{
  voxl::WindowDesc desc;
  voxl::Application app(desc);
  app.PushOverlay(std::make_unique<voxl::DevConsole>());
  app.PushLayer(std::make_unique<SandboxLayer>());
  app.Run();

  return 0;
}