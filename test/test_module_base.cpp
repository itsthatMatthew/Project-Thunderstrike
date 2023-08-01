#include <Arduino.h>
#include <unity.h>
#include "module.h"

void setUp() {    
}

void tearDown() {
}

void base_class_pass_state_transitions() {
    PTS::Module module("base_module");

    TEST_ASSERT_EQUAL(PTS::Module<>::INVALID, module.getState());

    module.passState();
    TEST_ASSERT_EQUAL(PTS::Module<>::ACTIVE, module.getState());

    module.passState();
    TEST_ASSERT_EQUAL(PTS::Module<>::PASSED, module.getState());

    module.passState();
    TEST_ASSERT_EQUAL(PTS::Module<>::PASSED, module.getState());

    module.failState();
    TEST_ASSERT_EQUAL(PTS::Module<>::PASSED, module.getState());
}

void base_class_fail_state_transitions() {
    PTS::Module module("base_module");

    TEST_ASSERT_EQUAL(PTS::Module<>::INVALID, module.getState());

    module.failState();
    TEST_ASSERT_EQUAL(PTS::Module<>::INVALID, module.getState());

    module.passState();
    TEST_ASSERT_EQUAL(PTS::Module<>::ACTIVE, module.getState());

    module.failState();
    TEST_ASSERT_EQUAL(PTS::Module<>::FAILED, module.getState());

    module.failState();
    TEST_ASSERT_EQUAL(PTS::Module<>::FAILED, module.getState());
    
    module.passState();
    TEST_ASSERT_EQUAL(PTS::Module<>::FAILED, module.getState());
}

void setup() {
    UNITY_BEGIN();

    RUN_TEST(base_class_pass_state_transitions);

    RUN_TEST(base_class_fail_state_transitions);

    UNITY_END();
}

void loop() {}