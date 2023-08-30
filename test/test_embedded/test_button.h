#include <gtest/gtest.h>
#include "utils/hw/button.h"

#pragma once

TEST(Button, released)
{
  PTS::Button<0> button(22);
  button.begin();
  
  digitalWrite(22, LOW);
  button.update();
  digitalWrite(22, LOW);

  ASSERT_EQ(PTS::Button<0>::StateChange::IS_RELEASED, button.getStateChange());
}

TEST(Button, rising)
{
  PTS::Button<0> button(22);
  button.begin();
  
  digitalWrite(22, LOW);
  button.update();
  digitalWrite(22, HIGH);

  ASSERT_EQ(PTS::Button<0>::StateChange::IS_RELEASED, button.getStateChange());
}

TEST(Button, pressed)
{
  PTS::Button<0> button(22);
  button.begin();
  
  digitalWrite(22, HIGH);
  button.update();
  digitalWrite(22, HIGH);

  ASSERT_EQ(PTS::Button<0>::StateChange::IS_RELEASED, button.getStateChange());
}

TEST(Button, falling)
{
  PTS::Button<0> button(22);
  button.begin();
  
  digitalWrite(22, HIGH);
  button.update();
  digitalWrite(22, LOW);

  ASSERT_EQ(PTS::Button<0>::StateChange::IS_RELEASED, button.getStateChange());
}
