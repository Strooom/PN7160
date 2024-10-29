// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <nci.hpp>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_initialize() {
    TEST_ASSERT_EQUAL(nciState::boot, nci::state);
    TEST_ASSERT_EQUAL(nciState::boot, nci::getState());
    TEST_ASSERT_EQUAL(tagPresentStatus::unknown, nci::tagsStatus);
    TEST_ASSERT_EQUAL(tagPresentStatus::unknown, nci::getTagPresentStatus());
}

void test_reset() {
    nci::moveState(nciState::waitForConfigResponse);
    nci::tagsStatus = tagPresentStatus::oldTagPresent;
    nci::reset();
    TEST_ASSERT_EQUAL(nciState::boot, nci::state);
    TEST_ASSERT_EQUAL(tagPresentStatus::unknown, nci::tagsStatus);
}

void test_check_status() {
    TEST_ASSERT_TRUE(nci::checkMessageStatus(0x00));
}

void test_state_machine() {
    nci::reset();
    nci::run();
    TEST_ASSERT_EQUAL(nciState::venResetActive, nci::getState());
    singleTimer::mockMillis += 5;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForResetDone, nci::getState());
    singleTimer::mockMillis += 5;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetResponse, nci::getState());
}

void test_dummy() {
    nci::moveState(nciState::boot);
    nci::getMessage();
    nci::checkMessageLength(0);
    nci::getMessageType(nci::rxBuffer);
    nci::getGroupIdentifier(nci::rxBuffer);
    nci::getOpcodeIdentifier(nci::rxBuffer);
    nci::getMessageId(nci::rxBuffer);
    nci::timeoutError();
    nci::unexpectedMessageError();
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_reset);
    RUN_TEST(test_check_status);
    RUN_TEST(test_state_machine);
    RUN_TEST(test_dummy);
    UNITY_END();
}
