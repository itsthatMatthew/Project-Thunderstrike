#ifndef BUTTON_HEADER_INCLUDED
#define BUTTON_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Own constants to keep track of a button's state.
// (the problem with arduino's implementation is that HIGH and RISING states are conflicting,
// enums are wonky and kinda kill it without their specific headers,
// are #defines better? genuinely not sure in this case?
// inline vars are c++17, only gives a warning but keep in mind)
const inline byte IS_RISING   = 0b01;
const inline byte IS_FALLING  = 0b10;
const inline byte IS_PRESSED  = 0b11;
const inline byte IS_RELEASED = 0b00;

// Utility class to link button state changes to callback functions
class Button {
 public:
  Button(uint8_t pin) : pin_(pin), state_(LOW),
    on_rising_(nullptr),
    on_falling_(nullptr),
    on_pressed_(nullptr),
    on_released_(nullptr)
   { }
  
  // sets up the communication pin and reads a beginning state
  void begin() {
  	pinMode(pin_, INPUT);
    read();
  }
  
  // reads the new state, stores and returns it
  byte read() { return state_ = digitalRead(pin_); }

  // determines what state change has happened to the button,
  // can detect rising and falling as well as simple high and low states (press and release)
  byte getStateChange() {
    byte change = static_cast<byte>(0b00);
    if (state_ == HIGH) change = static_cast<byte>(change | 0b10); // old state
    if (read() == HIGH) change = static_cast<byte>(change | 0b01); // new state
    return change;
  }
  
  // executes the correct callback depending on the buttons state and/or change
  void update() {
    switch (getStateChange()) {
      case IS_RISING: if (on_rising_) on_rising_(); break;
      case IS_FALLING: if (on_falling_) on_falling_(); break;
      case IS_PRESSED: if (on_pressed_) on_pressed_(); break;
      case IS_RELEASED: if (on_released_) on_released_(); break;
    }
  }
  
  void onRising(void (*callback)(void)) { on_rising_ = callback; }
  void onFalling(void (*callback)(void)) { on_falling_ = callback; }
  void onPressed(void (*callback)(void)) { on_pressed_ = callback; }
  void onReleased(void (*callback)(void)) { on_released_ = callback; }
  
 private:
  // inner vars:
  uint8_t pin_; // the connected gpio pin
  byte state_;  // the buttons state (since it's last update, used to calculate state changes)
  // callbacks:
  void (*on_rising_)(void);
  void (*on_falling_)(void);
  void (*on_pressed_)(void);
  void (*on_released_)(void);
}; /* class Button */

}; /* namespace PTS */

#endif /* BUTTON_HEADER_INCLUDED */