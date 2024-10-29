#include <Arduino.h>
#include <unity.h>
#include "singletimer.h"    

void test_singleTimer_isRunning() {
    constexpr unsigned long timeoutValue{10U};
    constexpr unsigned long timeoutDelta{2U};
    singleTimer theTimer;
    TEST_ASSERT_FALSE(theTimer.isRunning());    // after creation, the timer is not running
    theTimer.start(timeoutValue);
    TEST_ASSERT_TRUE(theTimer.isRunning());     // after starting, it is running, but not yet expired
    TEST_ASSERT_FALSE(theTimer.expired());
    delay(timeoutValue - timeoutDelta);
    TEST_ASSERT_TRUE(theTimer.isRunning());     // before timeoutValue has passed in [ms], it is still running, not yet expired
    TEST_ASSERT_FALSE(theTimer.expired());
    delay(timeoutDelta + timeoutDelta);
    TEST_ASSERT_TRUE(theTimer.expired());       // after timeoutValue, it is expired and no longer running
    TEST_ASSERT_FALSE(theTimer.isRunning());
}

void test_singleTimer_start_stop() {
    constexpr unsigned long timeoutValue{10U};
    singleTimer theTimer;
    TEST_ASSERT_FALSE(theTimer.isRunning());
    theTimer.start(timeoutValue);
    TEST_ASSERT_TRUE(theTimer.isRunning());
    TEST_ASSERT_FALSE(theTimer.expired());
    theTimer.stop();
    TEST_ASSERT_FALSE(theTimer.isRunning());
    TEST_ASSERT_FALSE(theTimer.expired());
}

void test_singleTimer_countdown() {
    constexpr unsigned long timeoutValue{10U};
    singleTimer theTimer;
    theTimer.start(timeoutValue);
    TEST_ASSERT_TRUE(theTimer.duration() == timeoutValue);
    TEST_ASSERT_TRUE(theTimer.value() == 0);
    delay(1U);
    TEST_ASSERT_TRUE(theTimer.duration() == timeoutValue);
    TEST_ASSERT_TRUE(theTimer.value() == 1U);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_singleTimer_isRunning);
    RUN_TEST(test_singleTimer_start_stop);
    RUN_TEST(test_singleTimer_countdown);
    UNITY_END();
}

void loop() {
}
