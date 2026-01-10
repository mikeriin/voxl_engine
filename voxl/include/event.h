#ifndef VOXL_EVENT_H
#define VOXL_EVENT_H


#include <cstdint>


namespace voxl {

enum class EventType: uint16_t {
  None,
  WindowClose,
  WindowResize,
  KeyPressed,
  KeyReleased,
  MouseMotion,
};


struct Event {
  EventType type = EventType::None;
  bool handled = false;
  virtual ~Event() = default;
};


struct WindowCloseEvent: Event {
  WindowCloseEvent() { type = EventType::WindowClose; }
};


struct WindowResizeEvent: Event {
  int width = 0;
  int height = 0;

  WindowResizeEvent(int w, int h)
    : width(w),
      height(h) 
    {
      type = EventType::WindowResize;
    }
};


struct KeyEvent: Event {
  int scancode = 0;
  int key = 0;
  uint16_t keymods = 0;
  bool repeat = false;
  
  KeyEvent(int sc, int k, uint16_t km, bool r)
    : scancode(sc),
      key(k),
      keymods(km),
      repeat(r)
    { }
};


struct KeyPressedEvent final: KeyEvent {
  KeyPressedEvent(int sc, int k, uint16_t km, bool r)
    : KeyEvent(sc, k, km, r)
    {
      type = EventType::KeyPressed;
    }
};


struct KeyReleasedEvent final: KeyEvent {
  KeyReleasedEvent(int sc, int k, uint16_t km)
    : KeyEvent(sc, k, km, false)
    {
      type = EventType::KeyReleased;
    }
};


}


#endif //! VOXL_EVENT_H