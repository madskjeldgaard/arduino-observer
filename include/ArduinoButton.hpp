#pragma once

#include <Debouncer.h>
#include <memory>
#include <utility>
#include "Observable.hpp"
#include "Arduino.h"
#include "String.h"

namespace obs {

/**
 * @class ArduinoButton
 * @brief An observable Arduino button class. Contains debouncing and allows for registering callbacks for when the button is pressed.
 *
 */
class ArduinoButton : public obs::Observable<ArduinoButton> {
 public:
  explicit ArduinoButton(int pin, int pinBehaviour = INPUT_PULLUP,
                         int debounceTime = 50)
      : mPin(pin), mDebouncer(pin, debounceTime) {
    pinMode(pin, pinBehaviour);

    mDebouncer.subscribe(Debouncer::Edge::FALL,
                         [&](const int /*state*/) { notify(*this, "fall"); });
    mDebouncer.subscribe(Debouncer::Edge::RISE,
                         [&](const int /*state*/) { notify(*this, "rise"); });
    mDebouncer.subscribe(Debouncer::Edge::CHANGED,
                         [&](const int /*state*/) { notify(*this, "value"); });
  }

  ~ArduinoButton() = default;

  void update() { mDebouncer.update(); }

  auto get() { return mDebouncer.read(); }

  auto getDebouncer() { return &mDebouncer; }

 private:
  int mPin;
  Debouncer mDebouncer;
};

class ButtonObserver : public Observer<ArduinoButton> {
 public:
  explicit ButtonObserver(std::function<void(ArduinoButton&, const String&)> callback) : mCallback(callback) {}

  ~ButtonObserver() = default;

  void field_changed(ArduinoButton& source,
                     const String& field_name) override {
    mCallback(source, field_name);
  }

 private:
  std::function<void(ArduinoButton&, const String&)> mCallback;
};

}
