//===-- utils/hw/rgbled.h - RGBLED utility class definition ---------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the RGBLED class, which is a
/// utility class for working with 3 + 1 legged RGB LEDs, where the 3 legs
/// represent red, green and blue.
///
/// Load balancing is your responsibility!
/// R : G : B resistors *should* be around 18 : 4 : 3
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_HW_RGBLED_H
#define UTILS_HW_RGBLED_H

#include <Arduino.h>

namespace PTS
{

/// RGBLED class
class RGBLED
{
//===-- Instantiation specific functions ----------------------------------===//
 public:
  explicit RGBLED(
    const uint8_t red_pin,
    const uint8_t green_pin,
    const uint8_t blue_pin
  )
  : c_red_pin(red_pin),
    c_green_pin(green_pin),
    c_blue_pin(blue_pin)
  { }

  /// Sets up the communication pins.
  void begin() const
  {
    pinMode(c_red_pin, OUTPUT);
    pinMode(c_green_pin, OUTPUT);
    pinMode(c_blue_pin, OUTPUT);
  }

//===-- Modifier functions ------------------------------------------------===//

  /// Sets the color red.
  void red() const
  {
    digitalWrite(c_red_pin, HIGH);
    digitalWrite(c_green_pin, LOW);
    digitalWrite(c_blue_pin, LOW);
  }

  /// Sets the color green.
  void green() const
  {
    digitalWrite(c_red_pin, LOW);
    digitalWrite(c_green_pin, HIGH);
    digitalWrite(c_blue_pin, LOW);
  }

  /// Sets the color blue.
  void blue() const
  {
    digitalWrite(c_red_pin, LOW);
    digitalWrite(c_green_pin, LOW);
    digitalWrite(c_blue_pin, HIGH);
  }

  /// Sets the color cyan.
  void cyan() const
  {
    digitalWrite(c_red_pin, LOW);
    digitalWrite(c_green_pin, HIGH);
    digitalWrite(c_blue_pin, HIGH);
  }

  /// Sets the color yellow.
  void yellow() const
  {
    digitalWrite(c_red_pin, HIGH);
    digitalWrite(c_green_pin, HIGH);
    digitalWrite(c_blue_pin, LOW);
  }

  /// Sets the color magenta.
  void magenta() const
  {
    digitalWrite(c_red_pin, HIGH);
    digitalWrite(c_green_pin, LOW);
    digitalWrite(c_blue_pin, HIGH);
  }

  /// Sets the color white by turning all LEDs on.
  void white() const
  {
    digitalWrite(c_red_pin, HIGH);
    digitalWrite(c_green_pin, HIGH);
    digitalWrite(c_blue_pin, HIGH);
  }

  /// Turns the LED off.
  void off() const
  {
    digitalWrite(c_red_pin, LOW);
    digitalWrite(c_green_pin, LOW);
    digitalWrite(c_blue_pin, LOW);
  }

//===-- Member variables --------------------------------------------------===//
 private:
  const uint8_t c_red_pin;
  const uint8_t c_green_pin;
  const uint8_t c_blue_pin;
}; // class RGBLED

} // namespace PTS

#endif // UTILS_HW_RGBLED_H
