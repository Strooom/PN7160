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
    TEST_ASSERT_EQUAL(tagStatus::noTag, nci::theTagStatus);
    TEST_ASSERT_EQUAL(tagStatus::noTag, nci::getTagPresentStatus());
}

void test_reset() {
    nci::moveState(nciState::waitForConfigResponse);
    nci::theTagStatus = tagStatus::oldTag;
    nci::reset();
    TEST_ASSERT_EQUAL(nciState::boot, nci::state);
    TEST_ASSERT_EQUAL(tagStatus::noTag, nci::theTagStatus);
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

void test_state_machine_timeouts() {
    PN7160Interface::mockIrqPin = false;

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForCoreResetResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetResponse, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForCoreResetNotification;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetNotification, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitforCoreInitResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitforCoreInitResponse, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForConfigResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForConfigResponse, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForDiscoverResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForDiscoverResponse, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForRfDeactivationResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForRfDeactivationResponse, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForRfDeactivationNotification;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForRfDeactivationNotification, nci::getState());
    singleTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());
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
    nci::startDiscover();
    nci::configure();
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_reset);
    RUN_TEST(test_check_status);
    RUN_TEST(test_state_machine);
    RUN_TEST(test_state_machine_timeouts);
    RUN_TEST(test_dummy);
    UNITY_END();
}
