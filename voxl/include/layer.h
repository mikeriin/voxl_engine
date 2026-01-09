#ifndef VOXL_LAYER_H
#define VOXL_LAYER_H


#include "event.h"


namespace voxl {


class Application;


// c'est la logique de chaque couche de l'application
class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnAttach(Application& app) {}
  virtual void OnDetach() {}

  virtual void OnEvent(Event& e) {}
  virtual void OnUpdate(double dt, double alpha) {}
  virtual void OnFixedUpdate(double fixed_dt) {};
  virtual void OnRender() {}
};


}


#endif //! VOXL_LAYER_H