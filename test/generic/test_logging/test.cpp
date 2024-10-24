// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include "logging.hpp"

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    TEST_ASSERT_EQUAL_CHAR(0x00, logging::buffer[0]);
    TEST_ASSERT_FALSE(logging::isActive(logging::source::stateChanges));
    TEST_ASSERT_FALSE(logging::isActive(logging::destination::uart1));
    logging::enable(logging::destination::uart1);
    logging::enable(logging::source::stateChanges);
    logging::reset();
    TEST_ASSERT_FALSE(logging::isActive(logging::source::stateChanges));
    TEST_ASSERT_FALSE(logging::isActive(logging::destination::uart1));
}

void test_enable_disable() {
    logging::reset();
    TEST_ASSERT_FALSE(logging::isActive(logging::source::stateChanges));
    logging::enable(logging::source::stateChanges);
    TEST_ASSERT_TRUE(logging::isActive(logging::source::stateChanges));
    logging::disable(logging::source::stateChanges);
    TEST_ASSERT_FALSE(logging::isActive(logging::source::stateChanges));

    TEST_ASSERT_FALSE(logging::isActive(logging::destination::uart1));
    logging::enable(logging::destination::uart1);
    TEST_ASSERT_TRUE(logging::isActive(logging::destination::uart1));
    logging::disable(logging::destination::uart1);
    TEST_ASSERT_FALSE(logging::isActive(logging::destination::uart1));
}

void test_output() {
    logging::reset();
    TEST_ASSERT_EQUAL(0, logging::snprintf("test"));        // before any destination is enabled
    logging::enable(logging::destination::uart1);           //
    TEST_ASSERT_EQUAL(4, logging::snprintf("test"));        // after a destination is enabled
}

void test_output_source() {
    logging::reset();
    logging::enable(logging::destination::uart1);
    TEST_ASSERT_EQUAL(0, logging::snprintf(logging::source::error, "test"));        // before any source is enabled
    logging::enable(logging::source::error);                                        //
    TEST_ASSERT_EQUAL(4, logging::snprintf(logging::source::error, "test"));        // after a source is enabled
}

void test_toString_destination() {
    TEST_ASSERT_EQUAL_STRING("uart1", toString(logging::destination::uart1));
    TEST_ASSERT_EQUAL_STRING("unknown", toString(static_cast<logging::destination>(99U)));
}

void test_toString_source() {
    TEST_ASSERT_EQUAL_STRING("stateChanges", toString(logging::source::stateChanges));
    TEST_ASSERT_EQUAL_STRING("error", toString(logging::source::error));
    TEST_ASSERT_EQUAL_STRING("criticalError", toString(logging::source::criticalError));
    TEST_ASSERT_EQUAL_STRING("unknown", toString(static_cast<logging::source>(99U)));
}

void test_dump() {
    logging::dump();
    TEST_IGNORE_MESSAGE("For Coverage Only");
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_enable_disable);
    RUN_TEST(test_output);
    RUN_TEST(test_output_source);
    RUN_TEST(test_toString_destination);
    RUN_TEST(test_toString_source);
    RUN_TEST(test_dump);
    UNITY_END();
}
