#ifndef VOXL_CORE_APPLICATION_H
#define VOXL_CORE_APPLICATION_H


#include <memory>

#include "input.h"
#include "layer_stack.h"
#include "window.h"
#include "renderer.h"
#include "timestep.h"
#include "resource_manager.h"
#include "dev_console.h"


namespace voxl {

struct AppContext {
  IWindow* pWindow = nullptr;
  IRenderer* pRenderer = nullptr;
  Timestep* pTimestep = nullptr;
  LayerStack* pLayerStack = nullptr;
  Input* pInput = nullptr;
  ResourceManager* pResManager = nullptr;
  IDevConsole* pDevConsole = nullptr;
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
  Timestep _timestep;
  LayerStack _layerStack;
  Input _input;
  ResourceManager _resManager;
  NullConsole _nullConsole;
  
  AppContext _context;

  void onEvent(Event& e);
};

}


#endif //! VOXL_CORE_APPLICATION_H