#include "layer_stack.h"


#include <utility>


namespace voxl {


void LayerStack::PushLayer(std::unique_ptr<Layer> layer) {
  _layers.insert(_layers.begin() + _insertIndex, std::move(layer));
  ++_insertIndex;
}


void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay) {
  _layers.push_back(std::move(overlay));
}


}