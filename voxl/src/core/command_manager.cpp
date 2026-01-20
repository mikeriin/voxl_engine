#include "command_manager.h"


namespace voxl {


bool CommandManager::RegisterCommand(std::string name, Command cmd) {
  return _commands.emplace(std::move(name), std::move(cmd)).second;
}


bool CommandManager::ExecuteCommand(std::string_view name, std::span<const std::string_view> args) const {
  auto it = _commands.find(std::string(name));
  if (it == _commands.end()) return false;
  return it->second.func(args);
}


std::string CommandManager::Help() const {
  std::string buffer;

  for (const auto& [key, val] : _commands) {
    buffer += key + " --> " + val.helper + "\n";
  }
  buffer.resize(buffer.size() - 1);

  return buffer;
}


std::string CommandManager::Helper(std::string_view name) const {
  auto it = _commands.find(std::string(name));
  if (it == _commands.end()) return "This command doesn't exist.";
  return it->second.helper;
}


}