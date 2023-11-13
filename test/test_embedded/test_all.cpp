#include <Arduino.h>
#include <gtest/gtest.h>
#include "test_led.h"
#include "test_button.h"
#include "test_circular_buffer.h"
#include "test_stateful_base.h"
#include "test_module_base.h"

void setup()
{
  Serial.begin(115200);

  ::testing::InitGoogleTest();
  
  if (RUN_ALL_TESTS())
  { }
}

void loop() { }