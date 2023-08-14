#ifndef BLINKER_MODULE_HEADER_INCLUDED
#define BLINKER_MODULE_HEADER_INCLUDED

#include "utils/led.h"

namespace PTS {

template<uint32_t BLINK_DURATION, uint32_t BLINK_PAUSE>
class BlinkerModule : Module {
  explicit BlinkerModule(const char *const name, const uint8_t pin)
    : Module(name), blinker(pin)
  { }
  
  void begin() const override {
    blinker.begin();
  }

  void threadFunc() const override {
    blinker.on();
    vTaskDelay(BLINK_DURATION / portTICK_PERIOD_MS);
    blinker.off();
    vTaskDelay(BLINK_PAUSE / portTICK_PERIOD_MS);
  }
 private:
  const LED blinker;
}; /* class BlinkerModule */

}; /* namespace PTS */

#endif /* BLINKER_MODULE_HEADER_INCLUDED */