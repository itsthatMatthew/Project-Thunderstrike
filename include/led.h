#ifndef LED_HEADER_INCLUDED
#define LED_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Utility class for 2 leg single color LEDs
class LED {
 public:
  LED(uint8_t pin) : pin_(pin) { }

  // sets up the communication pin
  void begin() {
  	pinMode(pin_, OUTPUT);
  }

  // turns the LED on
  void on() { digitalWrite(pin_, HIGH); }

  // turns the LED off
  void off() { digitalWrite(pin_, LOW); }

 private:
  uint8_t pin_;
}; /* class LED */

}; /* namespace PTS */

#endif /* LED_HEADER_INCLUDED */