#include <Arduino.h>
#include "modules/basic_wire_disconnect.h" 
#include "modules/hw/buzzer_module.h"
#include "utils/hw/led.h"
#include "modules/hw/keypad_module.h"
#include "bundles/counter_strike/counter_strike_styled.h"

const PTS::CounterStrikeStyledBomb bomb;

void setup() {
  Serial.begin(115200);

  bomb.begin();
  bomb.start();
}

void loop() {
  // No current functionality is necessary as "bomb" runs on a created thread.
  // The Arduino framework could be swapped in the future in order to get rid of
  // unnecessary boilerplate code.
}