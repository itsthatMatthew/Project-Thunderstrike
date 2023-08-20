//===-- module/basic_wire_disconnect.h - WireDisconnect class definition --===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the WireDisconnect class, which
/// is a module for a simple wire-disconnect game.
///
/// This file is also intended to be an example of how to write your own modules
///
/// To complete the module, you must disconnect the wires in the order passed to
/// the module's constructor. If all 3 wires have been disconnected, the module
/// is passed. If a wire has been disconnected in the wrong order, the module is
/// failed.
///
//===----------------------------------------------------------------------===//

#ifndef MODULE_BASIC_WIRE_DISCONNECT_H
#define MODULE_BASIC_WIRE_DISCONNECT_H

#include "module.h"
#include "utils/button.h"
#include "utils/rgbled.h"

namespace PTS
{

/// WireDisconnect class
class WireDisconnect : public Module<>
{
//===-- Instantiation specific functions ----------------------------------===//

 public:
  explicit WireDisconnect(const char *const name,
                          uint8_t wire_1, uint8_t wire_2, uint8_t wire_3,
                          const RGBLED &led_ref)
  : Module(name),
    c_wire_1(wire_1),
    c_wire_2(wire_2),
    c_wire_3(wire_3),
    c_status_rgbled(led_ref)
    { }
  
  /// Begin members and set callbacks.
  void begin() const override
  {
    c_wire_1.begin();
    c_wire_2.begin();
    c_wire_3.begin();
    c_status_rgbled.begin();

    // If wire_1 is disconnected, accumulate and set the disconnected value to 1
    c_wire_1.onFalling([](const WireDisconnect* obj_ptr) constexpr
    {
      Serial.println("Wire 1 disconnected!");
      obj_ptr->m_accumulator++;
      obj_ptr->m_disconnected = 1;
    });
    // If wire_2 is disconnected, accumulate and set the disconnected value to 2
    c_wire_2.onFalling([](const WireDisconnect* obj_ptr) constexpr
    {
      Serial.println("Wire 2 disconnected!");
      obj_ptr->m_accumulator++;
      obj_ptr->m_disconnected = 2;
    });
    // If wire_3 is disconnected, accumulate and set the disconnected value to 3
    c_wire_3.onFalling([](const WireDisconnect* obj_ptr) constexpr
    {
      Serial.println("Wire 3 disconnected!");
      obj_ptr->m_accumulator++;
      obj_ptr->m_disconnected = 3;
    });
    
    // Advance the module state from INVALID to ACTIVE
    this->passState();
    // Turn the status led on (blue color for live game)
    c_status_rgbled.blue();
  }

  /// Check for disconnected wires and run the game logic.
  void threadFunc() const override
  {
    // update each wire
    c_wire_1.update(this);
    c_wire_2.update(this);
    c_wire_3.update(this);

    // Game logic: if the number of wires disconnected does not match the number
    // of the currently disconnected one, fail
    if (m_accumulator != m_disconnected)
    {
      this->failState();
    }
    // But if all 3 wires have been disconnected, pass
    else if (m_disconnected == 3)
    {
      this->passState();
    }

    // If it's in a passing state, turn the led green
    if (this->getState() == PASSED)
    {
      Serial.println("Passed module!");
      c_status_rgbled.green();
    }
    // If it's in a failing state, turn the led red
    else if (this->getState() == FAILED)
    {
      Serial.println("Failed module!");
      c_status_rgbled.red();
    }

    // if any conditions met, delete the thread, as the game is finished
    if (this->getState() == PASSED || this->getState() == FAILED)
    {
      this->destroy();
    }
  }

//===-- Member variables --------------------------------------------------===//

 private:
  const Button<100, void, const WireDisconnect*> c_wire_1; 
  const Button<100, void, const WireDisconnect*> c_wire_2; 
  const Button<100, void, const WireDisconnect*> c_wire_3; 
  const RGBLED &c_status_rgbled;
  mutable size_t m_accumulator = 0;
  mutable size_t m_disconnected = 0;
}; // class WireDisconnect

} // namespace PTS

#endif // MODULE_BASIC_WIRE_DISCONNECT_H