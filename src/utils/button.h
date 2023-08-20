//===-- utils/button.h - Button utility class definition ------------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the Button class, which is a
/// utility class to link button state changes to callback functions
///
/// Callbacks are function pointers with template return and argument types.
/// Only one callback can be set to each state change.
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_BUTTON_H
#define UTILS_BUTTON_H

#include <Arduino.h>

namespace PTS
{

/// Button class
/// \tparam DELAY_MILLIS the desired software delay time for debouncing (in ms).
/// \tparam RETURN_TYPE the return type of the callbacks.
/// \tparam ARG_TYPES (variadic) the argument types of the callbacks.
template<
  uint32_t DELAY_MILLIS = 100,
  typename RETURN_TYPE = void, typename... ARG_TYPES
>
class Button
{
 public:
  /// Used to easily maintain the callback type within the class.
  using CALLBACK_TYPE = RETURN_TYPE(*)(ARG_TYPES...);

  /// Enum type for constants to keep track of a button's state (change).
  enum StateChange : uint8_t
  {
      IS_RELEASED = 0b00,
      IS_RISING   = 0b01,
      IS_FALLING  = 0b10,
      IS_PRESSED  = 0b11,
  };

//===-- Instantiation specific functions ----------------------------------===//

  explicit Button(const uint8_t pin)
  : c_pin(pin),
    m_state(LOW),
    m_delay_until(0),
    m_on_rising(nullptr),
    m_on_falling(nullptr),
    m_on_pressed(nullptr),
    m_on_released(nullptr)
   { }
  
  /// Sets up the communication pin and reads the beginning state.
  void begin() const
  {
    pinMode(c_pin, INPUT);
    readNewState();
  }
  
//===-- State change and callback specific functions ----------------------===//

  /// Reads the new state and stores it.
  /// If a delay is set, it discards new results until the set time.
  /// \return the read state.
  uint8_t readNewState() const
  {
    if (::millis() >= m_delay_until)
      m_state = digitalRead(c_pin);

    return m_state;
  }

  /// Returns the current state.
  /// \return the current state (HIGH or LOW).
  [[nodiscard]] uint8_t currentState() const { return m_state; }

  /// Determines what state change has happened to the button, and can detect
  /// rising and falling as well as pressed and released states.
  /// \return the determined state change.
  [[nodiscard]] StateChange getStateChange() const
  {
    StateChange change = static_cast<StateChange>(0b00);

    if (currentState() == HIGH) // old state
      change = static_cast<StateChange>(change | 0b10);

    if (readNewState() == HIGH) // new state
      change = static_cast<StateChange>(change | 0b01);

    if (change == RISING || change == FALLING) // only if changed
      m_delay_until = millis() + DELAY_MILLIS;

    return change;
  }
  
  /// Executes a callback depending on the buttons state change.
  /// \return the callback's result in the set return type, nothing if void.
  auto update(ARG_TYPES... args) const -> RETURN_TYPE
  {
    switch (getStateChange())
    {
      case IS_RISING:   if (m_on_rising)   return m_on_rising(args...);   break;
      case IS_FALLING:  if (m_on_falling)  return m_on_falling(args...);  break;
      case IS_PRESSED:  if (m_on_pressed)  return m_on_pressed(args...);  break;
      case IS_RELEASED: if (m_on_released) return m_on_released(args...); break;
    }

    // FIXME: in case no callback is set up yet, and control falls through,
    // *something* should be done here for non-void return types (complex
    // structures might not have default initializer)
  }
  
  /// Sets the callback called on rising state change.
  /// \param callback the callback to be set.
  void onRising(CALLBACK_TYPE callback) const { m_on_rising = callback; }

  /// Sets the callback called on falling state change.
  /// \param callback the callback to be set.
  void onFalling(CALLBACK_TYPE callback) const { m_on_falling = callback; }

  /// Sets the callback called on pressed state.
  /// \param callback the callback to be set.
  void onPressed(CALLBACK_TYPE callback) const { m_on_pressed = callback; }

  /// Sets the callback called on released state.
  /// \param callback the callback to be set.
  void onReleased(CALLBACK_TYPE callback) const { m_on_released = callback; }
  
//===-- Member variables --------------------------------------------------===//

 private:
  /// The connected gpio pin.
  const uint8_t c_pin;
  /// The buttons state.
  mutable uint8_t m_state;
  /// The time until the software delay should last.
  mutable unsigned long m_delay_until;

//===-- Callbacks ---------------------------------------------------------===//

 private:
  mutable CALLBACK_TYPE m_on_rising;
  mutable CALLBACK_TYPE m_on_falling;
  mutable CALLBACK_TYPE m_on_pressed;
  mutable CALLBACK_TYPE m_on_released;
}; // class Button

} // namespace PTS

#endif // UTILS_BUTTON_H
