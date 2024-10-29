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
    singleTimer::mockMillis = 500;
    singleTimer testTimer;
    testTimer.start(200);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_TRUE(testTimer.timerIsRunning);
    TEST_ASSERT_EQUAL(500, testTimer.startTime);
    TEST_ASSERT_EQUAL(200, testTimer.timerDuration);
    TEST_ASSERT_EQUAL(200, testTimer.duration());
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



int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_start);
    RUN_TEST(test_stop);
    RUN_TEST(test_expired);
    UNITY_END();
}