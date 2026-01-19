#ifndef VOXL_RESOURCE_H
#define VOXL_RESOURCE_H


#include <memory>
#include <concepts>


namespace voxl {


struct Resource {
  virtual ~Resource() = default;
};


template<typename T>
concept ResourceType = std::derived_from<T, Resource>;


template<ResourceType T>
struct Loader {
  template<typename... Args>
  static std::shared_ptr<T> Load(Args&&... args);
};


}


#endif //! VOXL_RESOURCE_H