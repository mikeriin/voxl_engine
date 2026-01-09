#ifndef VOXL_LAYER_STACK_H
#define VOXL_LAYER_STACK_H


#include <cstddef>
#include <memory>
#include <vector>

#include "layer.h"


namespace voxl {


class LayerStack {
public:
  void PushLayer(std::unique_ptr<Layer> layer);
  void PushOverlay(std::unique_ptr<Layer> overlay);

  auto Begin() { return _layers.begin(); }
  auto End() { return _layers.end(); }
  auto RBegin() { return _layers.rbegin(); }
  auto REnd() { return _layers.rend(); }

private:
  std::vector<std::unique_ptr<Layer>> _layers;
  size_t _insertIndex = 0;
};


}


#endif //! VOXL_LAYER_STACK_H