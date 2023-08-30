#include <gtest/gtest.h>
#include "utils/hw/led.h"

#pragma once

TEST(LED, on)
{
  PTS::LED led(22);
  led.begin();
  led.on();
  ASSERT_TRUE(digitalRead(22) == HIGH);
}

TEST(LED, off)
{
  PTS::LED led(22);
  led.begin();
  led.off();
  ASSERT_TRUE(digitalRead(22) == LOW);
}