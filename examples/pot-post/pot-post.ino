/*
 *
 * This example shows how to register a potentiometer as an observable and two
 * observers.
 *
 * The first observer posts the potentiometer value to the serial monitor.
 * The second observer sets the built in led brightness using PWM.
 *
 */
#include "ArduinoVoltage.hpp"
#include <Arduino.h>

// Button pin
constexpr auto POT_PIN = 23;
constexpr auto LED_PIN = LED_BUILTIN;

// Debounce time
constexpr auto DEBOUNCE_TIME = 50;

constexpr auto MAX_VALUE = 1023;

// Observable
obs::ArduinoVoltage potentiometer(POT_PIN);

// An observer posting the potentiometer value to the serial monitor
obs::VoltageObserver potPoster([](obs::ArduinoVoltage &pot,
                                  const String &field_name) {
  Serial.println("Potentiometer event: " + field_name);
  Serial.println("Potentiometer value: " + String(pot.getSmoothedValue()));
  Serial.println("Potentiometer normalized: " +
                 String(pot.getNormalizedValue()));
});

// An observer dimming the built-in LED when the poteniometer value changes
obs::VoltageObserver potBlinker([](obs::ArduinoVoltage &pot,
                                   const String &field_name) {
  const auto value = pot.getSmoothedValue();
  const auto brightness = map(value, 0, MAX_VALUE, 0, 255);
  Serial.println("Setting brightness to: " + String(brightness));
  analogWrite(LED_PIN, brightness);
});

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  potentiometer.subscribe(potPoster);
  potentiometer.subscribe(potBlinker);

  potentiometer.update();
}

void loop() { potentiometer.update(); }
