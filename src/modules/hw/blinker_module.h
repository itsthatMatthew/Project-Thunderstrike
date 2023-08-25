//===-- modules/blinker_module.h - Blinker module base class definition ---===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the BlinkerModule class, which
/// is a wrapper class for simple blinker hardware.
///
//===----------------------------------------------------------------------===//

#ifndef MODULES_HW_BLINKER_MODULE_H
#define MODULES_HW_BLINKER_MODULE_H

#include "modules/module_base.h"
#include "utils/hw/led.h"

namespace PTS
{

template<uint32_t BLINK_DURATION, uint32_t BLINK_PAUSE>
class BlinkerModule : public Module<>
{
//===-- Instantiation specific functions and threading function -----------===//
 public:
  explicit BlinkerModule(const std::string &name, const uint8_t pin)
  : Module(name), blinker(pin)
  { }
  
  /// Sets up blinker pin and makes the module active.
  void begin() const override
  {
    blinker.begin();
  }

  /// Turns the blinker on and off at the set intervals.
  void threadFunc() const override
  {
    blinker.on();
    ::delay(BLINK_DURATION);
    blinker.off();
    ::delay(BLINK_PAUSE);
  }

//===-- Member variable ---------------------------------------------------===//
 private:
  const LED blinker;
}; // class BlinkerModule

} // namespace PTS

#endif // MODULES_HW_BLINKER_MODULE_H