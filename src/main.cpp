#include <Arduino.h>
#include "utils/button.h"
#include "utils/led.h"
#include "utils/rgbled.h" 
#include "module.h"

using namespace PTS;

const LED led(LED_BUILTIN);
const Button button(GPIO_NUM_5);
Module module("module");

void setup() {
  led.begin();

  button.begin();

  button.onPressed([]() constexpr { led.on(); });
  button.onReleased([]() constexpr { led.off(); });

  module.begin();
  module.start();
}

void loop() {
  module.passState();
  module.failState();
  module.suspend();
  module.destroy();
  // loop
}