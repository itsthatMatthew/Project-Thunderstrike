//===-- utils/hw/status_bar.h - StatusBar class definition ----------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the StatusBar class, which is
/// a utility class for simple LED based status bars.
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_HW_STATUS_BAR_H
#define UTILS_HW_STATUS_BAR_H

#include <array>
#include "led.h"

namespace PTS {

/// StatusBar class
/// \tparam NUM the number of LEDs in the array.
template<size_t NUM>
class StatusBar {
//===-- Instantiation specific functions ----------------------------------===//
 public:
  explicit StatusBar(std::array<LED, NUM> &&led_array)
  : m_set_led(0), c_led_array(led_array)
  { }

  /// Sets up the LEDs in the array.
  void begin() const
  {
    for (auto led : c_led_array) led.begin();
  }

//===-- LED manipulation functions ----------------------------------------===//

  /// Turns the next LED in the array on.
  void next() const
  {
    if (m_set_led < NUM)
    {
      c_led_array[m_set_led++].on();
    }
  }

  /// Turns the last LED in the array off.
  void back() const
  {
    if (m_set_led > 0)
    {
      c_led_array[--m_set_led].off();
    }
  }

  /// Turns all LEDs in the array off.
  void clear() const
  {
    for (auto led : c_led_array) led.off();
    m_set_led = 0;
  }

//===-- Member variables --------------------------------------------------===//

 private:
  mutable size_t m_set_led;
  const std::array<LED, NUM> c_led_array;
}; // class StatusBar

} // namespace PTS

#endif // UTILS_HW_STATUS_BAR_H