#include "application.h"


#include <format>
#include <memory>
#include <print>
#include <string>
#include <utility>

#include "sdl_window.h"
#include "gl_renderer.h"
#include "command_manager.h"
#include "dev_console.h"
#include "event.h"
#include "timestep.h"
#include "res/shader_program.h"


std::string voxl::Loader<voxl::ShaderProgram>::_relativePath = "";


double voxl::Timestep::DeltaTime = 0.0;
double voxl::Timestep::FixedDeltaTime = 0.0;


// TODO permettre de choisir l'implémentation en dehors de la lib
std::unique_ptr<voxl::IWindow> CreateWindow(const voxl::WindowDesc& desc) { return std::make_unique<voxl::SDLWindow>(desc); }
std::unique_ptr<voxl::IRenderer> CreateRenderer(voxl::IWindow* window) { return std::make_unique<voxl::GLRenderer>(window); }


namespace voxl {


Application::Application(const voxl::WindowDesc &desc)
  : _pWindow(CreateWindow(desc)),
    _pRenderer(CreateRenderer(_pWindow.get()))
  {
    if (!_pWindow) _running = false;
    _pWindow->SetEventCallback([this](Event& e) { onEvent(e); });
    if (!_pRenderer) std::println("Failed to create renderer.");

    _timestep.SetFixedDeltaTime(1.0 / 60.0); // 60 mise à jours par secondes ~ 60fps

    _context.pWindow = _pWindow.get();
    _context.pRenderer = _pRenderer.get();
    _context.pResManager = &_resManager;
    _context.pTimestep = &_timestep;
    _context.pLayerStack = &_layerStack;
    _context.pInput = &_input;
    _context.pDevConsole = &_nullConsole;
    _context.pCmdManager = &_cmdManager;
  }


void Application::Run() {
  registerCommands();

  double fixed_timer = 0.0;
  while (_running) {
    _pWindow->PollEvents();
    
    _timestep.Update();
    fixed_timer += Timestep::DeltaTime;

    constexpr int MAX_STEP_COUNT = 8;
    int steps = 0;
    while (fixed_timer >= Timestep::FixedDeltaTime && ++steps < MAX_STEP_COUNT) {
      for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnFixedUpdate(Timestep::FixedDeltaTime);
      fixed_timer -= Timestep::FixedDeltaTime;
    }

    /*
    pour plus tard, calculer le alpha et mettre à jour la position (Render) en interpollant la position (Physics) précedente et l'actuelle

    double alpha = accumulator / FixedDeltaTime;
    auto renderPos = prevPos + (currPos - prevPos) * alpha;
    */
    double alpha = fixed_timer / Timestep::FixedDeltaTime;
    for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnUpdate(Timestep::DeltaTime, alpha);

    _pRenderer->BeginFrame();
    for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnRender();
    _pRenderer->EndFrame();
  }

  for (auto it = _layerStack.Begin(); it != _layerStack.End(); it++) (*it)->OnDetach();
}


void Application::Close() {
  _running = false;
}


void Application::PushLayer(std::unique_ptr<Layer> layer) {
  layer->OnAttach(this->_context);
  _layerStack.PushLayer(std::move(layer));
}


void Application::PushOverlay(std::unique_ptr<Layer> overlay) {
  overlay->OnAttach(this->_context);
  _layerStack.PushOverlay(std::move(overlay));
}


void Application::onEvent(Event& e) {
  if (e.type == EventType::WindowClose) {
    Close();
    e.handled = true;
    return;
  }

  if (e.type == EventType::WindowResize) {
    auto& window_resize_event = static_cast<WindowResizeEvent&>(e);

    std::string buffer = std::format("Window resized[{}, {}]", window_resize_event.width, window_resize_event.height);
    _context.pDevConsole->Send(DevConsoleMessage{
      .level = DevConsoleMessage::INFO,
      .buffer = buffer
    });
  }

  // on traverse les layers dans le sens inverse, la couche la plus haute a la priorité
  for (auto it = _layerStack.RBegin(); it != _layerStack.REnd(); it++) {
    (*it)->OnEvent(e);
    if (e.handled) break; // si l'event est marqué handled alors on ne l'utilise plus
  }

  if (e.type == EventType::KeyPressed || e.type == EventType::KeyReleased) _input.ProcessEvent(e);
}


void Application::registerCommands() {
  // enregistrement de la commande /quit qui ferme l'application
  _cmdManager.RegisterCommand("quit", Command{
    .helper = "/quit",
    .func = [this](std::span<const std::string_view> args) -> bool {
      if (args.size() != 0) {
        this->_context.pDevConsole->Send(DevConsoleMessage{
          .level = DevConsoleMessage::WARNING,
          .buffer = "Failed to execute /quit, too many arguments."
        });
        return false;
      }
      _running = false;
      return true;
    }
  });


  // commande print pour gérer plusieurs arguments
  _cmdManager.RegisterCommand("print", Command{
    .helper = "/print <your text to print here>",
    .func = [this](std::span<const std::string_view> args) -> bool {
      if (args.size() == 0) {
        this->_context.pDevConsole->Send(DevConsoleMessage{
          .level = DevConsoleMessage::WARNING,
          .buffer = "Failed to execute /print, need at least one argument."
        });
        return false;
      }

      std::string buffer;
      for (std::string_view token: args) {
        buffer.append(token.data(), token.size());
        buffer.push_back(' ');
      }

      this->_context.pDevConsole->Send(DevConsoleMessage{
        .level = DevConsoleMessage::NONE,
        .buffer = std::move(buffer)
      });

      return true;
    }
  });

}


}