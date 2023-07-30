#ifndef BUTTON_HEADER_INCLUDED
#define BUTTON_HEADER_INCLUDED

namespace PTS {

#include <Arduino.h>

// Utility class to link button state changes to callback functions
template<typename CALLBACK_TYPE = void(*)(void)>
class Button {
 public:
  // Own constants as enum to keep track of a button's state.
  enum StateChange : byte {
      IS_RISING   = 0b01,
      IS_FALLING  = 0b10,
      IS_PRESSED  = 0b11,
      IS_RELEASED = 0b00
  };

  explicit Button(const uint8_t pin) : pin_(pin), state_(LOW),
    on_rising_(nullptr),
    on_falling_(nullptr),
    on_pressed_(nullptr),
    on_released_(nullptr)
   { }
  
  // sets up the communication pin and reads a beginning state
  void begin() const {
    pinMode(pin_, INPUT);
    readNewState();
  }
  
  // reads the new state, stores and returns it
  byte readNewState() const { return state_ = digitalRead(pin_); }

  // returns the current state (HIGH or LOW)
  [[nodiscard]] byte currentState() const { return state_; }

  // determines what state change has happened to the button,
  // can detect rising and falling as well as simple high and low states (press and release)
  StateChange getStateChange() const {
    StateChange change = static_cast<StateChange>(0b00);
    if (currentState() == HIGH) change = static_cast<StateChange>(change | 0b10); // old state
    if (readNewState() == HIGH) change = static_cast<StateChange>(change | 0b01); // new state
    return change;
  }
  
  // executes the correct callback depending on the buttons state and/or change
  void update() const {
    switch (getStateChange()) {
      case IS_RISING: if (on_rising_) on_rising_(); break;
      case IS_FALLING: if (on_falling_) on_falling_(); break;
      case IS_PRESSED: if (on_pressed_) on_pressed_(); break;
      case IS_RELEASED: if (on_released_) on_released_(); break;
    }
  }
  
  void onRising(CALLBACK_TYPE callback) const { on_rising_ = callback; }
  void onFalling(CALLBACK_TYPE callback) const { on_falling_ = callback; }
  void onPressed(CALLBACK_TYPE callback) const { on_pressed_ = callback; }
  void onReleased(CALLBACK_TYPE callback) const { on_released_ = callback; }
  
 private:
  // inner vars:
  const uint8_t pin_; // the connected gpio pin
  mutable byte state_;  // the buttons state (since it's last update, used to calculate state changes)
  // callbacks:
  mutable CALLBACK_TYPE on_rising_;
  mutable CALLBACK_TYPE on_falling_;
  mutable CALLBACK_TYPE on_pressed_;
  mutable CALLBACK_TYPE on_released_;
}; /* class Button */

}; /* namespace PTS */

#endif /* BUTTON_HEADER_INCLUDED */
