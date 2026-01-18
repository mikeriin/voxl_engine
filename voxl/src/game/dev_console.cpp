#include "dev_console.h"


#include <cctype>
#include <format>
#include <print>
#include <string_view>
#include <vector>

#include <imgui.h>

#include "application.h"
#include "command_manager.h"
#include "event.h"
#include "keyboard.h"


namespace voxl {


bool DevConsole::_enable = false;
std::deque<std::string> DevConsole::_history{};
int DevConsole::_historyIndex = -1;


void DevConsole::OnAttach(AppContext& ctx) {
  _pCtx = &ctx;
  ctx.pDevConsole = this;
}


void DevConsole::OnEvent(Event& e) {
  if (e.type == EventType::KeyPressed) {
    const auto& key_pressed_event = static_cast<KeyPressedEvent&>(e);
    if (key_pressed_event.scancode == SCANCODE_DOLLAR && !_enable) {
      _enable = true;
      _justOpened = true;
    }
    if (key_pressed_event.scancode == SCANCODE_ESCAPE && _enable) {
      _enable = false;
      _historyIndex = -1;
      e.handled = true;
    }
  }
}


void DevConsole::OnUpdate([[maybe_unused]] double dt, [[maybe_unused]] double alpha) {
  if (_buffer.size() == 0) return;

  Send(DevConsoleMessage{
    .level = DevConsoleMessage::Level::NONE,
    .buffer = _buffer
  });

  if (_buffer[0] == COMMAND_PREFIX) {
    std::string_view line = _buffer;
    auto tokens = splitWords(line);

    if (!tokens.empty()) {
      std::string_view command_sv = tokens[0].substr(1, tokens[0].size() - 1);
      // on ignore le premier mot.
      std::span<const std::string_view> args(tokens.data() + 1, tokens.size() - 1);

      std::println("{}", args.size());

      bool executed = _pCtx->pCmdManager->ExecuteCommand(command_sv, args);
      if (executed) std::println("Command executed.");
    }
  }

  _buffer.clear();
}


void DevConsole::OnRender() {
  if (!_enable) return;

  ImGui::SetNextWindowSize(ImVec2(500.0f, 300.0f), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);

  unsigned int window_flags = 0;
  window_flags = ImGuiWindowFlags_NoDecoration;

  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);

  if (ImGui::Begin("DevConsole", &_enable, window_flags)) {
    if (_justOpened) {
      ImGui::SetKeyboardFocusHere(1);
      _justOpened = false;
    }

    static char str[512] = "";

    unsigned int input_text_flags = 0;
    input_text_flags |= ImGuiInputTextFlags_EnterReturnsTrue;
    input_text_flags |= ImGuiInputTextFlags_AutoSelectAll;
    input_text_flags |= ImGuiInputTextFlags_ElideLeft;
    input_text_flags |= ImGuiInputTextFlags_CallbackHistory;

    ImGui::PushItemWidth(-1.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));

    bool submitted = ImGui::InputTextWithHint("##DevConsoleInputText", "$", 
      str, IM_ARRAYSIZE(str), 
      input_text_flags, 
      DevConsole::historyCallback, this);

    ImGui::PopStyleColor(1);
    ImGui::PopItemWidth();

    if (submitted) {
      _buffer = str;
      str[0] = '\0';
      _historyIndex = -1;
      ImGui::SetKeyboardFocusHere(-1);
    }

    ImVec2 margin(5.0f, 5.0);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + margin.x, ImGui::GetCursorPos().y));
    ImVec2 avail_size = ImGui::GetContentRegionAvail();
    avail_size.x -= margin.x;
    avail_size.y -= margin.y;

    ImGui::BeginChild("##DevConsoleHistory", avail_size, ImGuiChildFlags_None);
    for (auto& line: _history) ImGui::TextWrapped("%s", line.c_str());
    ImGui::EndChild();
  }
  ImGui::End();

  ImGui::PopStyleVar(4);
  ImGui::PopStyleColor(1);
}


void DevConsole::Send(const DevConsoleMessage& message) {
  std::string level = "";
  switch (message.level) {
  case DevConsoleMessage::Level::INFO:
    level = "INFO ";
    break;

  case DevConsoleMessage::Level::WARNING:
    level = "WARNING ";
    break;

  case DevConsoleMessage::Level::ERROR:
    level = "ERROR ";
    break;

  default:
    break;
  }

  std::string line = std::format("{}> {}", level, message.buffer);

  _history.push_front(line);
  if (_history.size() > MAX_HISTORY_MESSAGE_COUNT) _history.pop_back();
}


std::string DevConsole::clearHistoryLine(const std::string& line) {
  std::string buffer = line;
  size_t pos = buffer.find(">");
  buffer.erase(0, pos + 2); // supprime "> " et tout ce qu'il y a avant
  return buffer;
}


int DevConsole::historyCallback(ImGuiInputTextCallbackData* data) {
  data->DeleteChars(0, data->BufTextLen);
  int history_size = _history.size();
  
  if (data->EventKey == ImGuiKey_DownArrow) {
    _historyIndex += 1;
    if (_historyIndex > history_size - 1) _historyIndex = history_size - 1;
    
    std::string text = clearHistoryLine(_history[_historyIndex]);
    data->InsertChars(0, text.c_str());
    data->SelectAll();
  }
  else if (data->EventKey == ImGuiKey_UpArrow) {
    _historyIndex -= 1;
    if (_historyIndex < 0) _historyIndex = 0;

    std::string text = clearHistoryLine(_history[_historyIndex]);
    data->InsertChars(0, text.c_str());
    data->SelectAll();
  }

  return 0;
}


std::vector<std::string_view> DevConsole::splitWords(std::string_view sv) {
  std::vector<std::string_view> out;
  size_t i = 0;

  while (i < sv.size()) {
    while (i < sv.size() && std::isspace((unsigned char)sv[i])) ++i;
    if (i >= sv.size()) break;

    const size_t start = i;
    while (i < sv.size() && !std::isspace((unsigned char)sv[i])) ++i;

    out.emplace_back(sv.data() + start, i - start);
    // begin = index du début du mot, end = taille du mot == index du charactère - index début du mot
  }

  return out;
}


}