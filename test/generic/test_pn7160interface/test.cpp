// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <pn7160interface.hpp>
#include <cstring>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    TEST_ASSERT_EQUAL(0, PN7160Interface::IRQpin);
    TEST_ASSERT_EQUAL(0, PN7160Interface::VENpin);
    TEST_ASSERT_EQUAL(0x28, PN7160Interface::I2Caddress);

    PN7160Interface::initialize(1, 2);

    TEST_ASSERT_EQUAL(1, PN7160Interface::IRQpin);
    TEST_ASSERT_EQUAL(2, PN7160Interface::VENpin);
    TEST_ASSERT_EQUAL(0x28, PN7160Interface::I2Caddress);

    PN7160Interface::initialize(3, 4, 0x50);

    TEST_ASSERT_EQUAL(3, PN7160Interface::IRQpin);
    TEST_ASSERT_EQUAL(4, PN7160Interface::VENpin);
    TEST_ASSERT_EQUAL(0x50, PN7160Interface::I2Caddress);
}

void test_hasMessage() {
    PN7160Interface::mockIrqPin = false;
    TEST_ASSERT_FALSE(PN7160Interface::hasMessage());
    PN7160Interface::mockIrqPin = true;
    TEST_ASSERT_TRUE(PN7160Interface::hasMessage());
}

void test_isWakeUp() {
    PN7160Interface::mockWakeUp = false;
    TEST_ASSERT_FALSE(PN7160Interface::wakeUp());
    PN7160Interface::mockWakeUp = true;
    TEST_ASSERT_TRUE(PN7160Interface::wakeUp());
}

void test_read() {
    const uint8_t testData[10]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint8_t actualData[10];
    memcpy(PN7160Interface::mockMessageData, testData, 10);
    TEST_ASSERT_EQUAL(5, PN7160Interface::read(actualData));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(testData, actualData, 5);
}

void test_write() {
    const uint8_t testData[10]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    TEST_ASSERT_EQUAL(0, PN7160Interface::write(testData, 10));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(testData, PN7160Interface::mockMessageData, 10);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_hasMessage);
    RUN_TEST(test_isWakeUp);
    RUN_TEST(test_read);
    RUN_TEST(test_write);
    UNITY_END();
}
