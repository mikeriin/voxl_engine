#include "timestep.h"
#include <algorithm>


namespace voxl {


void Timestep::Update() {
  auto current_frame_time = std::chrono::steady_clock::now();
  DeltaTime = std::min(std::chrono::duration<double>(current_frame_time - _lastFrameTime).count(), 0.25);
  _lastFrameTime = current_frame_time;
}


void Timestep::SetFixedDeltaTime(double fixedDT) {
  FixedDeltaTime = fixedDT;
}


}