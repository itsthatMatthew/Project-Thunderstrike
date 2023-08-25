//===-- utils/hw/led.h - LED utility class definition ---------------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the LED class, which is a
/// utility class for simlpe 2 leg single color LEDs.
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_HW_LED_H
#define UTILS_HW_LED_H

#include <Arduino.h>

namespace PTS
{

/// LED class 
class LED
{
//===-- Instantiation specific functions ----------------------------------===//
 public:
  explicit LED(const uint8_t pin) : c_pin(pin) { }

  /// Sets up the communication pin.
  void begin() const { pinMode(c_pin, OUTPUT); }

//===-- Modifier functions ------------------------------------------------===//

  /// Turns the LED on.
  void on() const { digitalWrite(c_pin, HIGH); }

  /// Turns the LED off.
  void off() const { digitalWrite(c_pin, LOW); }

//===-- Member variables --------------------------------------------------===//
 private:
  const uint8_t c_pin;
}; // class LED

} // namespace PTS

#endif // UTILS_HW_LED_H
