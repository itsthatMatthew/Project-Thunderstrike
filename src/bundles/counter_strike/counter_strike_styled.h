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

#include <string>

#include "utils/utils.h"
#include "modules/modules.h"
#include "modules/basic_wire_disconnect.h"

namespace PTS
{

static const size_t PASSCODE_MAX_SIZE = 4;

/// CounterStrikeStyledBomb class
/// Game logic: the bomb first has to be armed ("planted"), until that point, no
/// external force should set it off. Once it has been armed, a counter is set
/// off, there is exactly 40 seconds to disarm ("defuse") the bomb.
/// Arming can be done by entering an arbitrary sequence of numbers, to disarm
/// it, one has to short two specific pins.
class CounterStrikeStyledBomb : public Module<2024>, public Stateful
{
 public:
  CounterStrikeStyledBomb()
  : Module("cs_style_bomb"),
    Stateful(),
    c_blinker_led(LED_BUILTIN),
    c_state_led(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23),
    c_strikes_bar(std::array<LED, 3>{LED(GPIO_NUM_13),
                                     LED(GPIO_NUM_12),
                                     LED(GPIO_NUM_14)}),
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

  ~CounterStrikeStyledBomb()
  {
    c_keypad_module.destroy();
    c_wire_disconnect_module.destroy();
    c_buzzer_module.destroy();
  }

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
  }

  void threadFunc() const override
  {
    switch (getState())
    {
    case INVALID:
      if (auto read_char = c_keypad_module.readOne(); read_char)
      {
        const size_t passocde_size = entered_passcode.size();
        switch (read_char.value())
        {
        case '*': // backspace
          if (!entered_passcode.empty()) {
            entered_passcode.pop_back();
            if (passocde_size == PASSCODE_MAX_SIZE)
              c_ready_led.off();
            else
              c_strikes_bar.back();
            LOG::D("The current passcode is: %", entered_passcode.c_str());
          }
          else
            LOG::W("The passcode is already empty!");
          break;
        case '#': // enter
          if (passocde_size == PASSCODE_MAX_SIZE) {
            saved_passcode = entered_passcode;
            c_ready_led.off();
            c_strikes_bar.clear();
            c_armed_led.on();
            passState();
            LOG::I("Saved passcode: %", saved_passcode.c_str());
          }
          else {
            LOG::W("Cannot save passcode with less than % digits!",
                    PASSCODE_MAX_SIZE);
            LOG::D("The current passcode is: %", entered_passcode.c_str());
          }
          break;
        default:  // value
          if (passocde_size < PASSCODE_MAX_SIZE) {
            entered_passcode += read_char.value();
            if (passocde_size == PASSCODE_MAX_SIZE - 1)
              c_ready_led.on();
            else
              c_strikes_bar.next();
            LOG::D("The current passcode is: %", entered_passcode.c_str());
          }
          else {
            LOG::W("Cannot have a passcode with more than % digits!",
                    PASSCODE_MAX_SIZE);
            LOG::D("The current passcode is: %", entered_passcode.c_str());
          }
          break;
        }
      }
      break;

    case ACTIVE:
      // TODO
      break;

    default:
      // nothing on passed or failed state
      break;
    }

    // old main code:
    /*
    if (auto read_char = keypad.readOne(); read_char)
      Serial.print(read_char.value());

    if (wire_disconnect.getState() == PTS::Stateful::ACTIVE) {
      buzzer.resume();
      builting.on();

      delay(100);

      buzzer.suspend();
      builting.off();

      delay(buzzer_delay);

      if (buzzer_delay > 100) {
        buzzer_delay -= 10;
      }
      else if (buzzer_delay > 15) {
        buzzer_delay -= 5;
      }
      else if (buzzer_delay > 1) {
        buzzer_delay -= 1;
      }
      else {
        wire_disconnect.failState();
      }
    }
    else if (wire_disconnect.getState() == PTS::Stateful::FAILED) {
      if (static bool has_ran = false; !has_ran) { // once
        has_ran = true;
        buzzer.resume();
        delay(2000);
        buzzer.destroy();
        wire_disconnect.destroy();
      }
    }
    else if (wire_disconnect.getState() == PTS::Stateful::PASSED) {
      if (static bool has_ran = false; !has_ran) { // once
        has_ran = true;
        buzzer.resume();
        delay(50);
        buzzer.suspend();
        delay(50);
        buzzer.resume();
        delay(50);
        buzzer.suspend();
        delay(50);
        buzzer.resume();
        delay(50);
        buzzer.suspend();
        buzzer.destroy();
        wire_disconnect.destroy();
      }
    }
    */
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

  mutable std::string entered_passcode;
  mutable std::string saved_passcode;
}; // CounterStrikeStyledBomb

} // namespace PTS

#endif // BUNDLES_COUNTER_STRIKE_COUNTER_STRIKE_STYLED_H