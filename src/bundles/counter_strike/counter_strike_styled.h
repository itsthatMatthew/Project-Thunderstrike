//===-- bundles/counter_strike_counter_strike_styled.h -
//                             CounterStrikeStyledBomb class definition --===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the CounterStrikeStyledBomb
/// class, which is a bundle class containing several project modules
/// implementing a specific game logic to mimic the bomb behaviour in the
/// counter-strike franchise.
///
/// While the class does not implement a singleton behaviour, it should be
/// treated as such.
///
//===----------------------------------------------------------------------===//

#ifndef BUNDLES_COUNTER_STRIKE_COUNTER_STRIKE_STYLED_H
#define BUNDLES_COUNTER_STRIKE_COUNTER_STRIKE_STYLED_H

#include "utils/utils.h"
#include "modules/modules.h"
#include "modules/basic_wire_disconnect.h"

namespace PTS
{

/// CounterStrikeStyledBomb class
/// Game logic: the bomb first has to be armed ("planted"), until that point, no
/// external force should set it off. Once it has been armed, a counter is set
/// off, there is exactly 40 seconds to disarm ("defuse") the bomb.
/// Arming can be done by entering an arbitrary sequence of numbers, to disarm
/// it, one has to short two specific pins.
class CounterStrikeStyledBomb : public Module<>, public Stateful
{
 public:
  CounterStrikeStyledBomb()
  : Module("cs_style_bomb"),
    Stateful(),
    c_blinker_led(LED_BUILTIN),
    c_state_led(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23),
    c_strikes_bar(std::array<LED, 3>{LED(GPIO_NUM_13), LED(GPIO_NUM_12), LED(GPIO_NUM_14)}),
    c_ready_led(GPIO_NUM_27),
    c_armed_led(GPIO_NUM_26),
    c_keypad_module("cs_keypad", {25, 33, 32}, {35, 34, 39, 36},
                    std::array<std::array<char, 3>, 4>{
                      std::array<char, 3>{'1', '2', '3'},
                      std::array<char, 3>{'4', '5', '6'},
                      std::array<char, 3>{'7', '8', '9'},
                      std::array<char, 3>{'*', '0', '#'}}),
    c_wire_disconnect_module("cs_defuse", GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_4),
    c_buzzer_module("cs_buzzer", GPIO_NUM_19)
  { }


  void begin() const override
  {
    c_blinker_led.begin();
    c_state_led.begin();
    c_strikes_bar.begin();
    c_ready_led.begin();
    c_armed_led.begin();
    c_keypad_module.begin();
    c_wire_disconnect_module.begin();
    c_buzzer_module.begin();

    c_keypad_module.start();
    c_wire_disconnect_module.start();
    c_buzzer_module.start();

    passState();
  }

  void threadFunc() const override
  {
    //TODO
  }

 private:
  const LED           c_blinker_led;
  const RGBLED        c_state_led; 
  const StatusBar<3>  c_strikes_bar;
  const LED           c_ready_led;
  const LED           c_armed_led;
  const Keypad<3, 4>        c_keypad_module;
  const WireDisconnect      c_wire_disconnect_module;
  const BuzzerModule<10000> c_buzzer_module;
}; // CounterStrikeStyledBomb

} // namespace PTS

#endif // BUNDLES_COUNTER_STRIKE_COUNTER_STRIKE_STYLED_H