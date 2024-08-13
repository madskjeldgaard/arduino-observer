/*
 *
 * This example shows how to register a button and two observers to it.
 *
 * The first observer posts the button state to the serial monitor.
 * The second observer blinks the built-in LED when the button is pressed.
 *
 */
#include "ArduinoButton.hpp"
#include <Arduino.h>

// Button pin
constexpr auto BUTTON_PIN = 2;
constexpr auto LED_PIN = LED_BUILTIN;

// Debounce time
constexpr auto DEBOUNCE_TIME = 50;

// Observable
obs::ArduinoButton button(BUTTON_PIN, INPUT_PULLUP, DEBOUNCE_TIME);

// An observer posting the button state to the serial monitor
obs::ButtonObserver buttonPoster([](obs::ArduinoButton &button,
                                    const String &field_name) {
  Serial.println("Button event: " + field_name);
});

// An observer blinking the built-in LED when the button is pressed
obs::ButtonObserver buttonBlinker([](obs::ArduinoButton &button,
                                     const String &field_name) {
  if (field_name == "fall") {
    digitalWrite(LED_BUILTIN, HIGH);
  } else if (field_name == "rise") {
    digitalWrite(LED_BUILTIN, LOW);
  }
});

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  button.subscribe(buttonPoster);
  button.subscribe(buttonBlinker);

  button.update();
}

void loop() { button.update(); }
