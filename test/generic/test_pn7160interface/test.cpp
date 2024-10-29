// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <pn7160interface.hpp>

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

void test_dummy() {
    PN7160Interface::setVenPin(true);
    PN7160Interface::setVenPin(false);
    TEST_ASSERT_FALSE(PN7160Interface::wakeUp());
    TEST_ASSERT_TRUE(PN7160Interface::hasMessage());
    uint8_t data[10];
    TEST_ASSERT_EQUAL(0, PN7160Interface::read(data));
    TEST_ASSERT_EQUAL(PN7160Interface::i2cError, PN7160Interface::write(data, 10));
    TEST_ASSERT_EQUAL(0, PN7160Interface::write(data, 0));
    TEST_IGNORE_MESSAGE("For Coverage Only");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_dummy);
    UNITY_END();
}
