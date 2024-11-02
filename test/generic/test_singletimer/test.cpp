#include <unity.h>
#include "singletimer.hpp"

void setUp(void) {
}
void tearDown(void) {
}

void test_initialize() {
    singleShotTimer testTimer;
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_FALSE(testTimer.timerIsRunning);
    TEST_ASSERT_EQUAL(0, testTimer.startTime());
    TEST_ASSERT_EQUAL(0, testTimer.duration());
}

void test_standard_scenario() {        // Start timer, wait until it's expired

    singleShotTimer::mockMillis = 100;
    singleShotTimer testTimer;

    testTimer.start(50);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(50, testTimer.timeRemaining());

    singleShotTimer::mockMillis += 20;
    TEST_ASSERT_FALSE(testTimer.isExpired());
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(20, testTimer.timePassed());
    TEST_ASSERT_EQUAL(30, testTimer.timeRemaining());

    singleShotTimer::mockMillis += 40;
    TEST_ASSERT_TRUE(testTimer.isExpired());
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(0, testTimer.timeRemaining());

    TEST_ASSERT_FALSE(testTimer.isExpired());        // isExpired only returns true once
}

void test_early_stop_scenario() {        // Start timer, stop it before it expires
    singleShotTimer::mockMillis = 100;
    singleShotTimer testTimer;

    testTimer.start(50);
    singleShotTimer::mockMillis += 20;
    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isExpired());
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(0, testTimer.timeRemaining());
}

void test_start_or_continue() {
    singleShotTimer::mockMillis = 100;
    singleShotTimer testTimer;

    testTimer.start(50);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(50, testTimer.timeRemaining());

    singleShotTimer::mockMillis += 20;
    testTimer.startOrContinue(70);
    TEST_ASSERT_FALSE(testTimer.isExpired());
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(20, testTimer.timePassed());
    TEST_ASSERT_EQUAL(30, testTimer.timeRemaining());

    singleShotTimer::mockMillis += 40;
    TEST_ASSERT_TRUE(testTimer.isExpired());
    testTimer.startOrContinue(70);
    TEST_ASSERT_FALSE(testTimer.isExpired());
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(160, testTimer.startTime());
    TEST_ASSERT_EQUAL(70, testTimer.duration());
}

void test_expired_and_continue() {
    singleShotTimer::mockMillis = 100;
    singleShotTimer testTimer;

    testTimer.start(50);
    singleShotTimer::mockMillis += 20;
    TEST_ASSERT_FALSE(testTimer.isExpiredAndContinue());
    singleShotTimer::mockMillis += 40;
    TEST_ASSERT_TRUE(testTimer.isExpiredAndContinue());
    TEST_ASSERT_TRUE(testTimer.isRunning());

    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isExpiredAndContinue());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_standard_scenario);
    RUN_TEST(test_early_stop_scenario);
    RUN_TEST(test_start_or_continue);
    RUN_TEST(test_expired_and_continue);
    UNITY_END();
}

