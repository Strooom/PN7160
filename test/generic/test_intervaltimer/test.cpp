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
    TEST_ASSERT_EQUAL(0, testTimer.startTime());
    TEST_ASSERT_EQUAL(0, testTimer.duration());
}

void test_set() {
    intervalTimer testTimer;
    testTimer.set(1000);
    TEST_ASSERT_EQUAL_UINT(1000, testTimer.duration());
    TEST_ASSERT_FALSE(testTimer.isRunning());
}

void test_standard_scenario() {        // Start timer, wait until it's expired
    intervalTimer testTimer;
    intervalTimer::mockMillis = 100;

    testTimer.set(50);
    testTimer.start();
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(50, testTimer.timeRemaining());

    intervalTimer::mockMillis += 20;
    TEST_ASSERT_FALSE(testTimer.isExpired());
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(20, testTimer.timePassed());
    TEST_ASSERT_EQUAL(30, testTimer.timeRemaining());

    intervalTimer::mockMillis += 40;
    TEST_ASSERT_TRUE(testTimer.isExpired());
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(150, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(10, testTimer.timePassed());
    TEST_ASSERT_EQUAL(40, testTimer.timeRemaining());

    testTimer.stop();
    TEST_ASSERT_FALSE(testTimer.isRunning());
    TEST_ASSERT_FALSE(testTimer.isExpired());
}

void test_start_or_continue() {
    intervalTimer::mockMillis = 100;
    intervalTimer testTimer;

    testTimer.start(50);
    intervalTimer::mockMillis += 20;
    testTimer.startOrContinue();
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(100, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(20, testTimer.timePassed());
    TEST_ASSERT_EQUAL(30, testTimer.timeRemaining());

    intervalTimer::mockMillis += 20;

    testTimer.stop();
    testTimer.startOrContinue();
    TEST_ASSERT_TRUE(testTimer.isRunning());
    TEST_ASSERT_EQUAL(140, testTimer.startTime());
    TEST_ASSERT_EQUAL(50, testTimer.duration());
    TEST_ASSERT_EQUAL(0, testTimer.timePassed());
    TEST_ASSERT_EQUAL(50, testTimer.timeRemaining());
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_set);
    RUN_TEST(test_standard_scenario);
    RUN_TEST(test_start_or_continue);
    UNITY_END();
}
