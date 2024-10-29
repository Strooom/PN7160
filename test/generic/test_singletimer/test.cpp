#include <unity.h>
#include "singletimer.hpp"

void setUp(void) {
}
void tearDown(void) {
}

void test_initialize() {
    singleTimer testTimer;
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_FALSE(testTimer.timerIsRunning);
}

void test_start() {
    singleTimer::mockMillis = 100;
    singleTimer testTimer;
    testTimer.start(200);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_TRUE(testTimer.timerIsRunning);
    TEST_ASSERT_EQUAL(100, testTimer.startTime);
    TEST_ASSERT_EQUAL(200, testTimer.timerDuration);
    TEST_ASSERT_EQUAL(200, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.value());
    singleTimer::mockMillis += 100;
    TEST_ASSERT_EQUAL(100, testTimer.value());
}

void test_start_or_continue() {
    singleTimer::mockMillis = 100;
    singleTimer testTimer;
    testTimer.start(200);
    singleTimer::mockMillis += 100;
    testTimer.startOrContinue(400);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime);
    TEST_ASSERT_EQUAL(200, testTimer.timerDuration);
    TEST_ASSERT_EQUAL(200, testTimer.duration());
    testTimer.stop();
    singleTimer::mockMillis += 100;
    testTimer.startOrContinue(400);
    TEST_ASSERT_EQUAL(300, testTimer.startTime);
    TEST_ASSERT_EQUAL(400, testTimer.timerDuration);
}


void test_stop() {
    singleTimer::mockMillis = 500;
    singleTimer testTimer;
    testTimer.start(200);
    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isRunning());
}

void test_expired() {
    singleTimer::mockMillis = 500;
    singleTimer testTimer;
    testTimer.start(200);
    TEST_ASSERT_FALSE(testTimer.expired());
    singleTimer::mockMillis = 700;
    TEST_ASSERT_FALSE(testTimer.expired());
    singleTimer::mockMillis = 701;
    TEST_ASSERT_TRUE(testTimer.expired());
}

void test_expired_and_continue() {
    singleTimer::mockMillis = 500;
    singleTimer testTimer;
    testTimer.start(200);
    singleTimer::mockMillis = 700;
    TEST_ASSERT_FALSE(testTimer.expiredAndContinue());
    singleTimer::mockMillis = 701;
    TEST_ASSERT_TRUE(testTimer.expiredAndContinue());
    TEST_ASSERT_TRUE(testTimer.isRunning());
}


int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_start);
    RUN_TEST(test_start_or_continue);
    RUN_TEST(test_stop);
    RUN_TEST(test_expired);
    RUN_TEST(test_expired_and_continue);
    UNITY_END();
}