#ifndef VOXL_RESOURCE_LOADER_H
#define VOXL_RESOURCE_LOADER_H


namespace voxl {


template<typename T>
struct ResourceLoader {
  template<typename ...Args>
  static T Load(Args... args);
};


}


#endif //! VOXL_RESOURCE_LOADER_H