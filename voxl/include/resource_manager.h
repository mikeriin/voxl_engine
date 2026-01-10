#ifndef VOXL_RESOURCE_MANAGER_H
#define VOXL_RESOURCE_MANAGER_H


#include <print>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>

#include "resource.h"
#include "resource_loader.h"


namespace voxl {


class ResourceManager {
  using Res = std::shared_ptr<Resource>;

public:
  ~ResourceManager();

  template<typename T, typename ...Args>
  std::shared_ptr<T> Load(const std::string& name, Args&&... args);

  template<typename T>
  Res Get(const std::string& name);

  template<typename T, typename ...Args>
  std::shared_ptr<voxl::Resource> LoadOrGet(const std::string& name, Args&&... args);

private:
  std::unordered_map<std::string, Res> _resources;
};


}


template<typename T, typename ...Args>
inline std::shared_ptr<T> voxl::ResourceManager::Load(const std::string& name, Args&&... args) {
  auto loaded = std::make_shared<T>(std::move(ResourceLoader<T>::Load(std::forward<Args>(args)...)));

  if (loaded) {
    _resources.insert_or_assign(name, loaded);
    return loaded;
  }

  return nullptr;
}


template<typename T>
inline std::shared_ptr<voxl::Resource> voxl::ResourceManager::Get(const std::string& name) {
  auto it = _resources.find(name);
  if (it != _resources.end()) return it->second;

  std::println("This resource[{}] doesn't exist.", name);
  return nullptr;
}


template<typename T, typename ...Args>
inline std::shared_ptr<voxl::Resource> voxl::ResourceManager::LoadOrGet(const std::string& name, Args&&... args) {
  auto it = _resources.find(name);
  if (it != _resources.end()) return it->second;

  auto loaded = std::make_shared<T>(std::move(ResourceLoader<T>::Load(std::forward<Args>(args)...)));
  _resources.insert(std::make_pair(name, loaded));
  return loaded;
}


#endif //! VOXL_RESOURCE_MANAGER_H