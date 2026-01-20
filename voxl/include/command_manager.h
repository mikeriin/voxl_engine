#ifndef VOXL_COMMAND_MANAGER_H
#define VOXL_COMMAND_MANAGER_H


#include <functional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>


namespace voxl {


inline constexpr char COMMAND_PREFIX = '/'; 


using CommandHandler = std::function<bool(std::span<const std::string_view>)>;


struct Command {
  std::string helper;
  CommandHandler func;
};


class CommandManager {
public:
  bool RegisterCommand(std::string name, Command cmd);
  bool ExecuteCommand(std::string_view name, std::span<const std::string_view> args) const;

  std::string Help() const;
  std::string Helper(std::string_view name) const;

private:
  std::unordered_map<std::string, Command> _commands;
};


}


#endif //! VOXL_COMMAND_MANAGER_H