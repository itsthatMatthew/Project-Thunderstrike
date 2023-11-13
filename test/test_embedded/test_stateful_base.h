#include <gtest/gtest.h>
#include "modules/stateful_base.h"

#pragma once

TEST(Stateful, pass_transitions)
{
    PTS::Stateful stateful_class{};

    ASSERT_EQ(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.passState();
    ASSERT_EQ(PTS::Stateful::ACTIVE, stateful_class.getState());

    stateful_class.passState();
    ASSERT_EQ(PTS::Stateful::PASSED, stateful_class.getState());

    stateful_class.passState();
    ASSERT_EQ(PTS::Stateful::PASSED, stateful_class.getState());

    stateful_class.failState();
    ASSERT_EQ(PTS::Stateful::PASSED, stateful_class.getState());
}

TEST(Stateful, fail_transitions)
{
    PTS::Stateful stateful_class{};

    ASSERT_EQ(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.failState();
    ASSERT_EQ(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.passState();
    ASSERT_EQ(PTS::Stateful::ACTIVE, stateful_class.getState());

    stateful_class.failState();
    ASSERT_EQ(PTS::Stateful::FAILED, stateful_class.getState());

    stateful_class.failState();
    ASSERT_EQ(PTS::Stateful::FAILED, stateful_class.getState());
    
    stateful_class.passState();
    ASSERT_EQ(PTS::Stateful::FAILED, stateful_class.getState());
}