#include "input.h"


#include "event.h"


namespace voxl {


void Input::ProcessEvent(Event& e) {
  if (e.type == EventType::KeyPressed) {
    auto event = static_cast<KeyPressedEvent&>(e);
    if (event.repeat) return;

    int scancode = event.scancode;

    _keyHeld.set(scancode);
    _keyPressed.set(scancode);
  }
  if (e.type == EventType::KeyReleased) {
    auto event = static_cast<KeyReleasedEvent&>(e);
    int scancode = event.scancode;

    _keyHeld.reset(scancode);
    _keyReleased.set(scancode);
  }
}


bool Input::IsKeyPressed(Scancode scancode) {
  bool result = _keyPressed.test(static_cast<int>(scancode));
  _keyPressed.reset(static_cast<int>(scancode));
  return result;
}


bool Input::IsKeyHeld(Scancode scancode) const {
  return _keyHeld.test(static_cast<int>(scancode));
}


bool Input::IsKeyReleased(Scancode scancode) {
  bool result = _keyReleased.test(static_cast<int>(scancode));
  _keyReleased.reset(static_cast<int>(scancode));
  return result;
}


}