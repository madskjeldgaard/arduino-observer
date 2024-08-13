#pragma once

#include "Observable.hpp"
#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <functional>
#include <memory>

namespace obs {

/**
 * @class ArduinoVoltage
 * @brief An observable Arduino analog voltage reader with smoothing. Useful for
 * potentiometers, LDRs etc.
 *
 */
class ArduinoVoltage : public obs::Observable<ArduinoVoltage> {
public:
  ArduinoVoltage(int pin, bool sleepEnable = true)
      : mVoltage(pin, sleepEnable) {
    constexpr auto bits = 10;
    setAnalogResolution(bits);
  }

  ArduinoVoltage(int pin, int bits, bool sleepEnable = true)
      : mVoltage(pin, sleepEnable) {
    setAnalogResolution(bits);
  }

  auto getRawValue() -> auto { return mVoltage.getRawValue(); }

  auto getSmoothedValue() -> auto { return mVoltage.getValue(); }

  auto getResponsiveReader() -> auto { return &mVoltage; }

  auto getNormalizedValue() -> auto {
    return static_cast<float>(mVoltage.getValue()) /
           static_cast<float>(maxValue);
  }

  void setAnalogResolution(int bits) {
    resolutionBits = bits;
    maxValue = pow(2, bits);
    mVoltage.setAnalogResolution(maxValue);
  }

  auto getAnalogResolution() -> auto { return resolutionBits; }

  auto update() {
    mVoltage.update();
    if (mVoltage.hasChanged()) {
      notify(*this, "value");
    }
  }

private:
  ResponsiveAnalogRead mVoltage;
  int resolutionBits = 10;
  int maxValue = 1023;
};

// Takes a lambda and calls it when the voltage changes
class VoltageObserver : public Observer<ArduinoVoltage> {
public:
  explicit VoltageObserver(
      std::function<void(ArduinoVoltage &, const String &)> callback)
      : mCallback(callback) {}

  void field_changed(ArduinoVoltage &source,
                     const String &field_name) override {
    // if (field_name == "value") {
    mCallback(source, field_name);
    // }
  }

private:
  std::function<void(ArduinoVoltage &, const String &)> mCallback;
};

} // namespace obs
