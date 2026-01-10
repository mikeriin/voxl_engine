#ifndef VOXL_INPUT_H
#define VOXL_INPUT_H


#include "keyboard.h"

#include <bitset>


namespace voxl {


struct Event;


class Input {
public:
  ~Input() = default;

  void ProcessEvent(Event& e);

  bool IsKeyPressed(Scancode scancode);
  bool IsKeyHeld(Scancode scancode) const;
  bool IsKeyReleased(Scancode scancode);

private:
  std::bitset<512> _keyPressed;
  std::bitset<512> _keyReleased;
  std::bitset<512> _keyHeld;
};


}


#endif //! VOXL_INPUT_H