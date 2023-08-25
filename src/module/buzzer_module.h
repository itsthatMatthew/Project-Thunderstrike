//===-- module/buzzer_module.h - Buzzer module base class definition ------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the BuzzerModule class, which
/// is a wrapper class for simple buzzer hardware.
///
//===----------------------------------------------------------------------===//

#ifndef MODULE_BUZZER_MODULE_H
#define MODULE_BUZZER_MODULE_H

#include "module.h"

namespace PTS
{

/// BuzzerModule class
/// \tparam BUZZER_TONE the desired tone in frequency (Hz). 
template<uint32_t BUZZER_TONE>
class BuzzerModule : public Module<>
{
//===-- Instantiation specific functions and threading function -----------===//
 public:
  explicit BuzzerModule(const std::string &name, const uint8_t pin)
  : Module(name), c_buzzer_pin(pin)
  { }

  /// Sets up buzzer pin and makes the module active.
  void begin() const override
  {
    pinMode(c_buzzer_pin, OUTPUT);
  }

  /// Sends power to the buzzer pin at the set regular intervals.
  void threadFunc() const override
  {
    digitalWrite(c_buzzer_pin, HIGH);
    ::delayMicroseconds(/*1/f=*/ 1000000 / BUZZER_TONE);
    digitalWrite(c_buzzer_pin, LOW);
    ::delayMicroseconds(/*1/f=*/ 1000000 / BUZZER_TONE);
  }

//===-- Member variable ---------------------------------------------------===//
 private:
  const uint8_t c_buzzer_pin;
}; // class Buzzer

} // namespace PTS

#endif // MODULE_BUZZER_MODULE_H