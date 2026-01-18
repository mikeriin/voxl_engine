#ifndef VOXL_DEV_CONSOLE_H
#define VOXL_DEV_CONSOLE_H


#include <deque>
#include <string>

#include <imgui.h>
#include <string_view>
#include <vector>

#include "layer.h"


namespace voxl {


const int MAX_HISTORY_MESSAGE_COUNT = 150;


struct DevConsoleMessage {
  enum Level {
    NONE,
    INFO,
    WARNING,
    ERROR,
  };

  Level level;
  std::string buffer;
};


class IDevConsole {
public:
  virtual ~IDevConsole() = default;
  virtual void Send(const DevConsoleMessage& message) = 0;
};


class DevConsole final: public Layer, public IDevConsole {
public:
  void OnAttach(AppContext& ctx) override;
  // virtual void OnDetach() {}

  void OnEvent(Event& e) override;
  void OnUpdate(double dt, double alpha) override;
  // virtual void OnFixedUpdate(double fixed_dt) {};
  void OnRender() override;

  void Send(const DevConsoleMessage& message) override;

private:
  static bool _enable;
  bool _justOpened = false;
  AppContext* _pCtx = nullptr;
  std::string _buffer;
  static int _historyIndex;
  static std::deque<std::string> _history;

  static std::string clearHistoryLine(const std::string& line);
  static int historyCallback(ImGuiInputTextCallbackData* data);
  std::vector<std::string_view> splitWords(std::string_view sv);
};


class NullConsole final: public IDevConsole {
  void Send(const DevConsoleMessage& message) override {};
};


}


#endif //! VOXL_DEV_CONSOLE_H