#include <Arduino.h>
#include "module/basic_wire_disconnect.h"
#include "module/buzzer_module.h"
#include "utils/led.h"

PTS::WireDisconnect wire_disconnect("wire disconnect", GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_4);
PTS::BuzzerModule<1> buzzer("buzzer", GPIO_NUM_19);
PTS::LED builting(LED_BUILTIN);
uint32_t buzzer_delay = 1000;

void setup() {
  Serial.begin(115200);

  wire_disconnect.begin();
  buzzer.begin();
  builting.begin();

  wire_disconnect.start();
  buzzer.start();
  buzzer.suspend();
}

void loop() {
  if (wire_disconnect.getState() == PTS::Module<>::ACTIVE) {
    buzzer.resume();
    builting.on();
    delay(100);
    buzzer.suspend();
    builting.off();
    delay(buzzer_delay);

    if (buzzer_delay > 10) {
      buzzer_delay -= 10;
    }
    else {
      wire_disconnect.failState();
    }
  }
}