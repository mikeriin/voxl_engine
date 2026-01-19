#ifndef VOXL_UTILS_H
#define VOXL_UTILS_H


#include <print>
#include <sstream>
#include <string>
#include <fstream>


namespace voxl {


inline std::string ReadFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::println("Failed to open {}", path);
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}


}


#endif //! VOXL_UTILS_H