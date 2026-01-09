#include <voxl.h>

#include "sandbox.h"


int main(int argc, char* argv[]) 
{
  voxl::WindowDesc desc;
  voxl::Application app(desc);
  app.PushLayer(std::make_unique<SandboxLayer>());
  app.Run();

  return 0;
}