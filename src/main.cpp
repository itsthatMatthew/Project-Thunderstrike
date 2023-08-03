#include <Arduino.h>
#include "module/basic_wire_disconnect.h"

PTS::WireDisconnect module("module", GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_4);

void setup() {
  Serial.begin(115200);

  module.begin();
  module.start();
}

void loop() {
}