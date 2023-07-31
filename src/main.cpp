#include <Arduino.h>
#include "utils/button.h"
#include "utils/led.h"
#include "utils/rgbled.h" 

using namespace PTS;

const LED led(LED_BUILTIN);
const Button button(GPIO_NUM_5);

void setup() {
  led.begin();
  //led.on();

  button.begin();
  //Serial.begin(9600);

  button.onPressed([]() constexpr { led.on(); });
  button.onReleased([]() constexpr { led.off(); });

  //led.off();
}

void loop() {
  button.update();
  // loop
}