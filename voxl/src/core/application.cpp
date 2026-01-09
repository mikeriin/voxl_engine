#include "application.h"


#include <print>
#include <utility>

#include "SDL3Window.h"
#include "event.h"
#include "timestep.h"


double voxl::Timestep::DeltaTime = 0.0;
double voxl::Timestep::FixedDeltaTime = 0.0;


// TODO permettre de choisir l'implémentation en dehors de la lib
std::unique_ptr<voxl::IWindow> CreateWindow(const voxl::WindowDesc& desc) { return std::make_unique<voxl::SDL3Window>(desc); }


namespace voxl {

Application::Application(const voxl::WindowDesc &desc)
  : _pWindow(CreateWindow(desc))
  {
    if (!_pWindow) _running = false;
    _pWindow->SetEventCallback([this](Event& e) { onEvent(e); });

    _timestep.SetFixedDeltaTime(1.0 / 60.0); // 60 mise à jours par secondes ~ 60fps

    _context.pWindow = _pWindow.get();
    _context.pTimestep = &_timestep;
    _context.pLayerStack = &_layerStack;
  }


void Application::Run() {
  double fixedTimer = 0.0;
  while (_running) {
    _pWindow->PollEvents();
    
    _timestep.Update();
    fixedTimer += Timestep::DeltaTime;

    constexpr int MAX_STEP_COUNT = 8;
    int steps = 0;
    while (fixedTimer >= Timestep::FixedDeltaTime && ++steps < MAX_STEP_COUNT) {
      for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnFixedUpdate(Timestep::FixedDeltaTime);
      fixedTimer -= Timestep::FixedDeltaTime;
    }

    /*
    pour plus tard, calculer le alpha et mettre à jour la position (Render) en interpollant la position (Physics) précedente et l'actuelle

    double alpha = accumulator / FixedDeltaTime;
    auto renderPos = prevPos + (currPos - prevPos) * alpha;
    */
    double alpha = fixedTimer / Timestep::FixedDeltaTime;
    for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnUpdate(Timestep::DeltaTime, alpha);

    // renderer.beginFrame()
    for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnRender();
    // renderer.endFrame()

    _pWindow->SwapBuffers();
  }
}


void Application::Close() {
  _running = false;
}


void Application::PushLayer(std::unique_ptr<Layer> layer) {
  layer->OnAttach(*this);
  _layerStack.PushLayer(std::move(layer));
}


void Application::PushOverlay(std::unique_ptr<Layer> overlay) {
  overlay->OnAttach(*this);
  _layerStack.PushOverlay(std::move(overlay));
}


void Application::onEvent(Event& e) {
  if (e.type == EventType::WindowClose) {
    Close();
    e.handled = true;
    return;
  }

  if (e.type == EventType::WindowResize) {
    auto& windowResizeEvent = static_cast<WindowResizeEvent&>(e);
    std::println("Window resized[%d, %d]", windowResizeEvent.width, windowResizeEvent.height);
  }

  // on traverse les layers dans le sens inverse, la couche la plus haute a la priorité
  for (auto it = _layerStack.RBegin(); it != _layerStack.REnd(); it++) {
    (*it)->OnEvent(e);
    if (e.handled) break;
  }
}


}