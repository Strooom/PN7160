#include <unity.h>
#include <intervaltimer.hpp>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    intervalTimer testTimer;
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_FALSE(testTimer.timerIsRunning);
}

void test_set() {
    intervalTimer testTimer;
    testTimer.set(1000);
    TEST_ASSERT_EQUAL_UINT(1000, testTimer.interval());
    TEST_ASSERT_FALSE(testTimer.isRunning());
}

void test_start_stop() {
    intervalTimer testTimer;
    testTimer.start();
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_TRUE(testTimer.timerIsRunning);
    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isRunning());
}

void test_start_or_continue() {
    intervalTimer testTimer;
    testTimer.start(100);
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_TRUE(testTimer.timerIsRunning);
    intervalTimer::mockMillis += 50;
    TEST_ASSERT_EQUAL(50, testTimer.value());
    testTimer.startOrContinue();
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(0, testTimer.intervalStartTime);
    testTimer.stop();
    testTimer.startOrContinue();
    TEST_ASSERT_EQUAL(50, testTimer.intervalStartTime);
}

void test_expired() {
    intervalTimer testTimer;
    testTimer.start(100);
    TEST_ASSERT_FALSE(testTimer.expired());
    intervalTimer::mockMillis += 101;
    TEST_ASSERT_TRUE(testTimer.expired());
    TEST_ASSERT_FALSE(testTimer.expired());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_set);
    RUN_TEST(test_start_stop);
    RUN_TEST(test_start_or_continue);
    RUN_TEST(test_expired);

    UNITY_END();
}
