#include <Arduino.h>
#include "modules/basic_wire_disconnect.h" 
#include "modules/hw/buzzer_module.h"
#include "utils/hw/led.h"

PTS::RGBLED wire_disconnect_status_led(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23);
PTS::WireDisconnect wire_disconnect("wire disconnect",
                                    GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_4,
                                    wire_disconnect_status_led);
                                    
PTS::BuzzerModule<12000> buzzer("buzzer", GPIO_NUM_19);
PTS::LED builting(LED_BUILTIN);
uint32_t buzzer_delay = 1000;

void setup() {
  Serial.begin(115200);

  wire_disconnect.begin();
  buzzer.begin();
  builting.begin();

  wire_disconnect.start();
  buzzer.start();
}

void loop() {
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
    static bool has_ran = false;
    if (!has_ran) { // once
      has_ran = true;
      buzzer.resume();
      delay(2000);
      buzzer.destroy();
      wire_disconnect.destroy();
    }
  }
  else if (wire_disconnect.getState() == PTS::Stateful::PASSED) {
    static bool has_ran = false;
    if (!has_ran) { // once
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
}