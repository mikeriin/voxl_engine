#ifndef VOXL_RESOURCE_MANAGER_H
#define VOXL_RESOURCE_MANAGER_H


#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <typeindex>

#include "res/resource.h"


namespace voxl {


template<typename T>
using Cache = std::unordered_map<std::string, std::shared_ptr<T>>;


class ResourceManager {
public:
  template<ResourceType T, typename... Args>
  std::shared_ptr<T> Load(const std::string& name, Args&&... args);

  template<ResourceType T>
  std::shared_ptr<T> Get(const std::string& name);

private:
  std::unordered_map<std::type_index, std::any> _caches;

  template<typename T>
  Cache<T>& getCache();
};


template<ResourceType T, typename... Args>
inline std::shared_ptr<T> ResourceManager::Load(const std::string& name, Args&&... args) {
  auto& cache = getCache<T>();

  auto it = cache.find(name);
  if (it != cache.end()) 
    return it->second;
  
  auto res = Loader<T>::Load(std::forward<Args>(args)...);
  if (!res) return nullptr;
  cache.emplace(name, res);
  return res;
}


template<ResourceType T>
inline std::shared_ptr<T> ResourceManager::Get(const std::string& name) {
  auto& cache = getCache<T>();
  if (cache == std::bad_any_cast()) return nullptr;

  auto it = cache.find(name);
  if (it == cache.end()) return nullptr;

  return it.second;
}


template<typename T>
inline Cache<T>& ResourceManager::getCache() {
  const std::type_index key{typeid(T)};

  auto it = _caches.find(key);
  if (it == _caches.end()) 
    it = _caches.emplace(key, Cache<T>{}).first;

  return std::any_cast<Cache<T>&>(it->second);
}


}


#endif //! VOXL_RESOURCE_MANAGER_H