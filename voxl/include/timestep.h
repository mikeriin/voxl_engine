#ifndef VOXL_TIMESTEP_H
#define VOXL_TIMESTEP_H


#include <chrono>


namespace voxl {


class Timestep {
public:
  static double DeltaTime;
  static double FixedDeltaTime;

  void Update();

  void SetFixedDeltaTime(double fixedDT);

private:
  std::chrono::time_point<std::chrono::steady_clock> _lastFrameTime = std::chrono::steady_clock::now();
};


}


#endif //! VOXL_TIMESTEP_H