#ifndef VOXL_CORE_APPLICATION_H
#define VOXL_CORE_APPLICATION_H


#include <memory>

#include "layer_stack.h"
#include "window.h"
#include "timestep.h"


namespace voxl {

struct AppContext {
  IWindow* pWindow = nullptr;
  Timestep* pTimestep = nullptr;
  LayerStack* pLayerStack = nullptr;
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
  Timestep _timestep;
  LayerStack _layerStack;
  
  AppContext _context;

  void onEvent(Event& e);
};

}


#endif //! VOXL_CORE_APPLICATION_H