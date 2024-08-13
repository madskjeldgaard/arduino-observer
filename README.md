# Arduino Observer 

The [Observer Design Pattern](https://en.wikipedia.org/wiki/Observer_pattern) is a common design pattern used to define a subscription mechanism that notifies one or more objects of an event happening. 

In the world of microcontrollers, this is particularly useful when reading potentiometers, encoders or buttons. Often you will want several things happening when a user touches the hardware interface, and with the observer design pattern, all these different things are seperated in a neat way that greatly simplifies the interface for the programmer and makes it easier to maintain because different concerns are seperatable in the code.

For example: A user presses a button and you want this event to trigger an LED on/off, post a message to the serial interface, trigger a MIDI Note, update some internal state, etc.

This library makes that easier by supplying a simple Observer and Observable class as well as a few optional helper classes for registering events when button presses happen or potentiometer values change. These include debouncing and value smoothing.

# Features

- Simple Observer and Observerable classes when you want to design your own event observers and observables.
- Convenience classes for common interfaces:
    - `ArduinoButton` / `ButtonObserver` – Set up a button with a de-noising debouncer and listen for event changes
    - `ArduinoVoltage` / `VoltageObserver` – Set up an ADC input (like a potentiometer or an LDR), with smoothing including, and listen for event changes.

## Usage

See [examples](examples) for more usage examples. 

```cpp
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
```

### Platformio installation

Add the library to your lib_deps in `platformio.ini`

```
lib_deps = 
    https://github.com/madskjeldgaard/arduino-observer
```

### Dependencies

These dependencies are optional and only used if you use the `ArduinoButton` or `ArduinoVoltage` convenience classes.

If you install this library through platformio, they are automatically included.

- [ResponsiveAnalogRead](https://github.com/dxinteractive/ResponsiveAnalogRead) – for ADC smoothing
- [Debouncer](https://github.com/hideakitai/Debouncer) – for debouncing button presses

### Making your own classes

See the [include/ArduinoButton.hpp](include/ArduinoButton.hpp) class file for inspiration on how to make your own.
