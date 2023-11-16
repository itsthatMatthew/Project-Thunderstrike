#pragma once

#include <gtest/gtest.h>
#include "utils/sw/circular_buffer.h"

TEST(CircularBuffer, empty_ctor)
{
  PTS::CircularBuffer<int, 4> buffer = PTS::CircularBuffer<int, 4>();

  ASSERT_TRUE(buffer.empty());
}

TEST(CircularBuffer, init_list_ctor)
{
  PTS::CircularBuffer<int, 4> buffer{1, 2, 3};

  ASSERT_EQ(3, buffer.size());
  ASSERT_EQ(1, buffer.front());
  ASSERT_EQ(3, buffer.back());
}

TEST(CircularBuffer, push)
{
  PTS::CircularBuffer<int, 4> buffer = PTS::CircularBuffer<int, 4>();

  buffer.push(1);
  buffer.push(2);

  ASSERT_EQ(1, buffer.front());
  ASSERT_EQ(2, buffer.back());
}

TEST(CircularBuffer, pop)
{
  PTS::CircularBuffer<int, 4> buffer{1, 2, 3};

  buffer.pop();
  buffer.push(4);
  buffer.pop();

  ASSERT_EQ(3, buffer.front());
  ASSERT_EQ(4, buffer.back());
}

TEST(CircularBuffer, overwhelm)
{
  PTS::CircularBuffer<int, 4> buffer{1, 2, 3, 4};

  ASSERT_EQ(1, buffer.front());
  ASSERT_EQ(4, buffer.back());
  ASSERT_EQ(4, buffer.size());


  buffer.push(5);
  buffer.push(6);
  buffer.push(7);
  buffer.push(8);

  ASSERT_EQ(1, buffer.front());
  ASSERT_EQ(4, buffer.back());
  ASSERT_EQ(4, buffer.size());

  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();

  buffer.push(5);
  buffer.push(6);
  buffer.push(7);
  buffer.push(8);

  ASSERT_EQ(5, buffer.front());
  ASSERT_EQ(8, buffer.back());
  ASSERT_EQ(4, buffer.size());

  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();
  buffer.pop();

  buffer.push(1);
  ASSERT_EQ(1, buffer.front());
  ASSERT_EQ(1, buffer.back());
  ASSERT_EQ(1, buffer.size());

}