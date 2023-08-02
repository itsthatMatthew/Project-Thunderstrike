#ifndef BASIC_WIRE_DISCONNECT_HEADER_INCLUDED
#define BASIC_WIRE_DISCONNECT_HEADER_INCLUDED

#include "module.h"
#include "./utils/button.h"
#include "./utils/led.h"

namespace PTS {

class WireDisconnect : public Module<> {
 public:
  explicit WireDisconnect(const char *const name, uint8_t wire, uint8_t led)
    : Module(name), wire(wire), builtin(led)
    { }
  
  void begin() const override {
    wire.begin();
    builtin.begin();

    wire.onRising([](const LED led) { Serial.println("Connected!"); });
    wire.onFalling([](const LED led) { Serial.println("Disconnected!"); });
    wire.onPressed([](const LED led) { led.on(); });
    wire.onReleased([](const LED led) { led.off(); });
  }

  void threadFunc() const override {
    wire.update(builtin);
  }

 private:
  const Button<NoDelayPolicy, void, const LED> wire; 
  const LED builtin;
}; /* class WireDisconnect */

}; /* namespace PTS */

#endif /* BASIC_WIRE_DISCONNECT_HEADER_INCLUDED */