#ifndef SANDBOX_H
#define SANDBOX_H


#include <print>

#include "application.h"
#include "layer.h"


class SandboxLayer: public voxl::Layer {
public:
  void OnAttach(voxl::Application& app) override { _pApp = &app; }

  void OnUpdate(double dt, double alpha) override {
    std::println("dt: {} \t\t\t {}", dt, alpha);
  }

  void OnFixedUpdate(double fixed_dt) override {
    std::println("FixedUpdate.");
  }

private:
  voxl::Application* _pApp;
};


#endif //! SANDBOX_H