#include <unity.h>
#include "modules/stateful_base.h"

void setUp() { }

void tearDown() { }

void base_class_pass_state_transitions()
{
    PTS::Stateful stateful_class{};

    TEST_ASSERT_EQUAL(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.passState();
    TEST_ASSERT_EQUAL(PTS::Stateful::ACTIVE, stateful_class.getState());

    stateful_class.passState();
    TEST_ASSERT_EQUAL(PTS::Stateful::PASSED, stateful_class.getState());

    stateful_class.passState();
    TEST_ASSERT_EQUAL(PTS::Stateful::PASSED, stateful_class.getState());

    stateful_class.failState();
    TEST_ASSERT_EQUAL(PTS::Stateful::PASSED, stateful_class.getState());
}

void base_class_fail_state_transitions()
{
    PTS::Stateful stateful_class{};

    TEST_ASSERT_EQUAL(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.failState();
    TEST_ASSERT_EQUAL(PTS::Stateful::INVALID, stateful_class.getState());

    stateful_class.passState();
    TEST_ASSERT_EQUAL(PTS::Stateful::ACTIVE, stateful_class.getState());

    stateful_class.failState();
    TEST_ASSERT_EQUAL(PTS::Stateful::FAILED, stateful_class.getState());

    stateful_class.failState();
    TEST_ASSERT_EQUAL(PTS::Stateful::FAILED, stateful_class.getState());
    
    stateful_class.passState();
    TEST_ASSERT_EQUAL(PTS::Stateful::FAILED, stateful_class.getState());
}

void setup()
{
    UNITY_BEGIN();

    RUN_TEST(base_class_pass_state_transitions);

    RUN_TEST(base_class_fail_state_transitions);

    UNITY_END();
}

void loop() {}