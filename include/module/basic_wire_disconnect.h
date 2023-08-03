// This file is mainly aimed at being an example at how to write your own modules
#ifndef BASIC_WIRE_DISCONNECT_HEADER_INCLUDED
#define BASIC_WIRE_DISCONNECT_HEADER_INCLUDED

#include "module.h"
#include "./utils/button.h"
#include "./utils/rgbled.h"

namespace PTS {

// The derived module class, we don't want to specify any of the template arguments
class WireDisconnect : public Module<> {
 public:
  // base constructor and member variables init (the status led was hard coded)
  explicit WireDisconnect(const char *const name, uint8_t wire_1, uint8_t wire_2, uint8_t wire_3)
    : Module(name), wire_1(wire_1), wire_2(wire_2), wire_3(wire_3), status(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23)
    { }
  
  // begin function (override)
  void begin() const override {
    wire_1.begin();
    wire_2.begin();
    wire_3.begin();
    status.begin();

    // what to do, when wire_1 is disconnected (state is a falling edge)
    wire_1.onFalling([](const WireDisconnect* obj_ptr) constexpr {
      Serial.println("Wire 1 disconnected!");
      obj_ptr->accumulator++;
      obj_ptr->disconnect = 1;
    });
    // same for wire_2
    wire_2.onFalling([](const WireDisconnect* obj_ptr) constexpr {
      Serial.println("Wire 2 disconnected!");
      obj_ptr->accumulator++;
      obj_ptr->disconnect = 2;
    });
    // and wire_3
    wire_3.onFalling([](const WireDisconnect* obj_ptr) constexpr {
      Serial.println("Wire 3 disconnected!");
      obj_ptr->accumulator++;
      obj_ptr->disconnect = 3;
    });
    
    // at the end of begin(), advance the module state from INVALID to ACTIVE
    this->passState();
    // turn the status led on (blue color for live game)
    status.blue();
  }

  // the overridden threadFunc, this function is ran on the created task in a loop
  void threadFunc() const override {
    // update each wire (this might be done smarter as removing all 3 wires
    // simultaneously might result in a passing state)
    wire_1.update(this);
    wire_2.update(this);
    wire_3.update(this);

    // game logic: if the number of wires disconnected does not match the
    // sequence number of the currently disconnected one, fail
    if (accumulator != disconnect) {
      this->failState();
    }
    // else if all 3 wires have been disconnected, pass
    else if (disconnect == 3) {
      this->passState();
    }

    // if it's in a passing state, turn the led green
    if (this->getState() == PASSED) {
      Serial.println("Passed module!");
      status.green();
    }
    // else if it's in a failing state, turn the led red
    else if (this->getState() == FAILED) {
      Serial.println("Failed module!");
      status.red();
    }

    // if any conditions met, delete the thread, as the game is finished
    if (this->getState() == PASSED || this->getState() == FAILED) {
      this->destroy();
    }
  }

 private:
  const Button<ArduinoDelayPolicy<100>, void, const WireDisconnect*> wire_1; 
  const Button<ArduinoDelayPolicy<100>, void, const WireDisconnect*> wire_2; 
  const Button<ArduinoDelayPolicy<100>, void, const WireDisconnect*> wire_3; 
  const RGBLED status;
  mutable size_t accumulator = 0;
  mutable size_t disconnect = 0;
}; /* class WireDisconnect */

}; /* namespace PTS */

#endif /* BASIC_WIRE_DISCONNECT_HEADER_INCLUDED */