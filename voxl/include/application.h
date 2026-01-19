#ifndef VOXL_CORE_APPLICATION_H
#define VOXL_CORE_APPLICATION_H


#include <memory>

#include "input.h"
#include "layer_stack.h"
#include "resource_manager.h"
#include "window.h"
#include "gfx/renderer.h"
#include "timestep.h"
#include "dev_console.h"
#include "command_manager.h"


namespace voxl {

struct AppContext {
  IWindow* pWindow = nullptr;
  IRenderer* pRenderer = nullptr;
  ResourceManager* pResManager = nullptr;
  Timestep* pTimestep = nullptr;
  LayerStack* pLayerStack = nullptr;
  Input* pInput = nullptr;
  IDevConsole* pDevConsole = nullptr;
  CommandManager* pCmdManager = nullptr;
};


class Application {
public:
  explicit Application(const voxl::WindowDesc &desc);
  ~Application() = default;

  void Run();
  void Close();

  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> overlay);

private:
  bool _running = true;

  std::unique_ptr<IWindow> _pWindow;
  std::unique_ptr<IRenderer> _pRenderer;
  ResourceManager _resManager;
  Timestep _timestep;
  LayerStack _layerStack;
  Input _input;
  NullConsole _nullConsole;
  CommandManager _cmdManager;
  
  AppContext _context;

  void onEvent(Event& e);
  void registerCommands();
};

}


#endif //! VOXL_CORE_APPLICATION_H