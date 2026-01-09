#ifndef VOXL_EVENT_H
#define VOXL_EVENT_H


#include <cstdint>


namespace voxl {

enum class EventType: uint16_t {
  None,
  WindowClose,
  WindowResize,
  KeyDown,
  KeyHeld,
  KeyUp,
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
  int key = 0;
  
  KeyEvent(EventType t, int k)
    : key(k)
    {
      type = t;
    }
};


}


#endif //! VOXL_EVENT_H