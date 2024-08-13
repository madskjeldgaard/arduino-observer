#pragma once
#include <memory>
#include <Arduino.h>

namespace obs {

// An Observer base class for observing an object using the Observer design pattern
template <typename T>
class Observer {
 public:
  virtual void field_changed(T& source, const String& field_name) = 0;
};
}  // namespace obs
