#ifndef RGBLED_HEADER_INCLUDED
#define RGBLED_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Utility class for working with 3 + 1 leg RGB LEDs,
// where the 3 legs represent red, green and blue.
// CAUTION! Load balancing is your responsibility!
// R : G : B resistors *should* be around 18 : 4 : 3
class RGBLED {
 public:
  RGBLED(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : r_(red_pin), g_(green_pin), b_(blue_pin)
  { }

  // sets up the communication pins
  void begin() {
    pinMode(r_, OUTPUT);
    pinMode(g_, OUTPUT);
    pinMode(b_, OUTPUT);
  }

  // sets the color red
  void red() {
    digitalWrite(r_, HIGH);
    digitalWrite(g_, LOW);
    digitalWrite(b_, LOW);
  }

  // sets the color green
  void green() {
    digitalWrite(r_, LOW);
    digitalWrite(g_, HIGH);
    digitalWrite(b_, LOW);
  }

  // sets the color blue
  void blue() {
    digitalWrite(r_, LOW);
    digitalWrite(g_, LOW);
    digitalWrite(b_, HIGH);
  }

  // sets the color cyan
  void cyan() {
    digitalWrite(r_, LOW);
    digitalWrite(g_, HIGH);
    digitalWrite(b_, HIGH);
  }

  // sets the color yellow
  void yellow() {
    digitalWrite(r_, HIGH);
    digitalWrite(g_, HIGH);
    digitalWrite(b_, LOW);
  }

  // sets the color magenta
  void magenta() {
    digitalWrite(r_, HIGH);
    digitalWrite(g_, LOW);
    digitalWrite(b_, HIGH);
  }

  // sets the color white by turning all LEDs on
  void white() {
    digitalWrite(r_, HIGH);
    digitalWrite(g_, HIGH);
    digitalWrite(b_, HIGH);
  }

  // turns the LED off
  void off() {
    digitalWrite(r_, LOW);
    digitalWrite(g_, LOW);
    digitalWrite(b_, LOW);
  }

 private:
  uint8_t r_;
  uint8_t g_;
  uint8_t b_;
}; /* class RGBLED */

}; /* namespace PTS */

#endif /* RGBLED_HEADER_INCLUDED */