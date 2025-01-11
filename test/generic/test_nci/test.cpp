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

void test_getMessageType() {
    uint8_t msgBuffer[1] = {0x00};
    TEST_ASSERT_EQUAL(messageType::Data, nci::getMessageType(msgBuffer));
    msgBuffer[0] = 0x20;
    TEST_ASSERT_EQUAL(messageType::Command, nci::getMessageType(msgBuffer));
    msgBuffer[0] = 0x40;
    TEST_ASSERT_EQUAL(messageType::Response, nci::getMessageType(msgBuffer));
    msgBuffer[0] = 0x60;
    TEST_ASSERT_EQUAL(messageType::Notification, nci::getMessageType(msgBuffer));
}

void test_getGroupIdentifier() {
    uint8_t msgBuffer[1] = {0x00};
    TEST_ASSERT_EQUAL(groupIdentifier::Core, nci::getGroupIdentifier(msgBuffer));
    msgBuffer[0] = 0x01;
    TEST_ASSERT_EQUAL(groupIdentifier::RfManagement, nci::getGroupIdentifier(msgBuffer));
    msgBuffer[0] = 0x02;
    TEST_ASSERT_EQUAL(groupIdentifier::NfceeManagement, nci::getGroupIdentifier(msgBuffer));
    msgBuffer[0] = 0x0F;
    TEST_ASSERT_EQUAL(groupIdentifier::Proprietary, nci::getGroupIdentifier(msgBuffer));
}

void test_getOpcodeIdentifier() {
    uint8_t msgBuffer[2] = {0x00, 0X00};
    TEST_ASSERT_EQUAL(opcodeIdentifier::CORE_RESET_CMD, nci::getOpcodeIdentifier(msgBuffer));
    msgBuffer[1] = 0x01;
    TEST_ASSERT_EQUAL(opcodeIdentifier::CORE_INIT_CMD, nci::getOpcodeIdentifier(msgBuffer));
    msgBuffer[1] = 0x02;
    TEST_ASSERT_EQUAL(opcodeIdentifier::CORE_SET_CONFIG_CMD, nci::getOpcodeIdentifier(msgBuffer));
    msgBuffer[1] = 0x03;
    TEST_ASSERT_EQUAL(opcodeIdentifier::CORE_GET_CONFIG_CMD, nci::getOpcodeIdentifier(msgBuffer));
}

void test_getMessageId() {
    uint8_t msgBuffer0[2] = {0x20, 0x00};
    TEST_ASSERT_EQUAL(nciMessageId::CORE_RESET_CMD, nci::getMessageId(msgBuffer0));
    uint8_t msgBuffer1[2] = {0x40, 0x03};
    TEST_ASSERT_EQUAL(nciMessageId::CORE_GET_CONFIG_RSP, nci::getMessageId(msgBuffer1));
}    
 
// CORE_RESET_CMD : 20 00 01 00
// CORE_RESET_RSP : 40 00 01 00
// CORE_RESET_NTF : 60 00 09 02 00 20 04 04 61 12 50 05
// CORE_INIT_CMD : 20 01 02 00 00
// CORE_INIT_RSP : 40 01 1E 00 1A 7E 06 02 01 D0 02 FF FF 01 FF 00 08 00 00 01 00 02 00 03 00 80 00 82 00 83 00 84 00
// CORE_GET_CONFIG_CMD : 20 03 02 01 00
// CORE_GET_CONFIG_RSP : 40 03 06 00 01 00 02 E8 03

// CORE_GET_CONFIG_RSP : 40 03
// payload length 06
// status 00 
// number of parameter fields 01 
// tag-value ID 00 = TOTAL_DURATION
// length 02
// value E8 03

// CORE_GET_CONFIG_CMD : 20 03 03 01 A0 0E
// nci stateChange from waitforCoreInitResponse (5) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 10 00 01 A0 0E 0B 11 01 C1 B1 00 DA 1E 14 00 D0 0C

// 20 02 04 01 00 F4 01
// 20 02 CORE_SET_CONFIG_CMD
// 04 Len
// 01 Number TagValuePairs
// 00 TagValue ID

// F4 01

// CORE_RESET_CMD : 20 00 01 00
// nci stateChange from waitForHwReset (2) to waitForCoreResetResponse (3)
// CORE_RESET_RSP : 40 00 01 00
// nci stateChange from waitForCoreResetResponse (3) to waitForCoreResetNotification (4)
// CORE_RESET_NTF : 60 00 09 02 00 20 04 04 61 12 50 05
// CORE_INIT_CMD : 20 01 02 00 00
// nci stateChange from waitForCoreResetNotification (4) to waitforCoreInitResponse (5)
// CORE_INIT_RSP : 40 01 1E 00 1A 7E 06 02 01 D0 02 FF FF 01 FF 00 08 00 00 01 00 02 00 03 00 80 00 82 00 83 00 84 00
// CORE_GET_CONFIG_CMD : 20 03 02 01 00
// nci stateChange from waitforCoreInitResponse (5) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 06 00 01 00 02 E8 03
// configData mismatch : ToBe : F4 01 --- AsIs : E8 03
// CORE_SET_CONFIG_CMD : 20 02 05 01 00 02 F4 01
// nci stateChange from waitForGetConfigResponse (6) to waitForSetConfigResponse (7)
// CORE_SET_CONFIG_RSP : 40 02 02 00 00
// CORE_GET_CONFIG_CMD : 20 03 02 01 00
// nci stateChange from waitForSetConfigResponse (7) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 06 00 01 00 02 F4 01
// configData mismatch : ToBe : E8 03 --- AsIs : F4 01
// CORE_SET_CONFIG_CMD : 20 02 05 01 00 02 E8 03
// nci stateChange from waitForGetConfigResponse (6) to waitForSetConfigResponse (7)
// CORE_SET_CONFIG_RSP : 40 02 02 00 00
// CORE_GET_CONFIG_CMD : 20 03 02 01 28
// nci stateChange from waitForSetConfigResponse (7) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 05 00 01 28 01 00
// configLength ToBe 7 - AsIs 8
// CORE_SET_CONFIG_CMD : 20 02 03 01 28 00
// nci stateChange from waitForGetConfigResponse (6) to waitForSetConfigResponse (7)
// CORE_SET_CONFIG_RSP : 40 02 02 00 00
// Unknown Message : 21 03 07 03 00 01 01 01 02 01
// nci stateChange from waitForSetConfigResponse (7) to waitForDiscoverResponse (8)
// RF_DISCOVER_RSP : 41 03 01 00
// nci stateChange from waitForDiscoverResponse (8) to waitForRfInterfaceActivatedNotification (9)

// nci stateChange from boot (0) to venResetActive (1)
// nci stateChange from venResetActive (1) to waitForHwReset (2)
// CORE_RESET_CMD : 20 00 01 00 
// nci stateChange from waitForHwReset (2) to waitForCoreResetResponse (3)
// CORE_RESET_RSP : 40 00 01 00
// nci stateChange from waitForCoreResetResponse (3) to waitForCoreResetNotification (4)
// CORE_RESET_NTF : 60 00 09 02 00 20 04 04 61 12 50 05
// CORE_INIT_CMD : 20 01 02 00 00
// nci stateChange from waitForCoreResetNotification (4) to waitforCoreInitResponse (5)
// CORE_INIT_RSP : 40 01 1E 00 1A 7E 06 02 01 D0 02 FF FF 01 FF 00 08 00 00 01 00 02 00 03 00 80 00 82 00 83 00 84 00
// CORE_GET_CONFIG_CMD : 20 03 02 01 00
// nci stateChange from waitforCoreInitResponse (5) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 06 00 01 00 02 F4 01
// CORE_GET_CONFIG_CMD : 20 03 03 01 A0 0E
// nci stateChange from waitForGetConfigResponse (6) to waitForGetConfigResponse (6)
// CORE_GET_CONFIG_RSP : 40 03 10 00 01 A0 0E 0B 11 01 C2 B2 00 DA 1E 14 00 D0 0C
// RF_DISCOVER_CMD : 21 03 07 03 00 01 01 01 02 01
// nci stateChange from waitForGetConfigResponse (6) to waitForDiscoverResponse (8)
// RF_DISCOVER_RSP : 41 03 01 00
// nci stateChange from waitForDiscoverResponse (8) to waitForRfInterfaceActivatedNotification (9)
// Unknown Message : 61 23 00
// unexpected NCI message Unknown Message : 61 23 00
// nci stateChange from waitForRfInterfaceActivatedNotification (9) to error (13)
// nci stateChange from error (13) to error (13)

// RF_INTF_ACTIVATED_NTF : 61 05 1E 01 02 04 00 FF 01 0D 44 03 07 04 19 65 6A 32 5A 80 01 20 00 00 00 00 06 05 75 77 81 02 80 

void test_initialize() {
    TEST_ASSERT_EQUAL(nciState::boot, nci::state);
    TEST_ASSERT_EQUAL(nciState::boot, nci::getState());
    TEST_ASSERT_EQUAL(tagStatus::absent, nci::theTagStatus);
    TEST_ASSERT_EQUAL(tagStatus::absent, nci::getTagStatus());
}

void test_reset() {
    nci::moveState(nciState::waitForGetConfigResponse);
    nci::theTagStatus = tagStatus::old;
    nci::reset();
    TEST_ASSERT_EQUAL(nciState::boot, nci::state);
    TEST_ASSERT_EQUAL(tagStatus::absent, nci::theTagStatus);
}

void test_check_status() {
    TEST_ASSERT_TRUE(nci::checkMessageStatus(0x00));
}

void test_state_machine() {
    nci::reset();
    nci::run();
    TEST_ASSERT_EQUAL(nciState::venResetActive, nci::getState());
    singleShotTimer::mockMillis += 5;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForHwReset, nci::getState());
    singleShotTimer::mockMillis += 5;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetResponse, nci::getState());
}

void test_state_machine_timeouts() {
    PN7160Interface::mockIrqPin = false;

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForCoreResetResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetResponse, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForCoreResetNotification;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreResetNotification, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForCoreInitResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForCoreInitResponse, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForGetConfigResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForGetConfigResponse, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForDiscoverResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForDiscoverResponse, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForRfDeactivateResponse;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForRfDeactivateResponse, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
    nci::run();
    TEST_ASSERT_EQUAL(nciState::error, nci::getState());

    singleShotTimer::mockMillis = 0;
    nci::responseTimeoutTimer.start(nci::standardResponseTimeout);
    nci::state = nciState::waitForRfDeactivateNotification;
    nci::run();
    TEST_ASSERT_EQUAL(nciState::waitForRfDeactivateNotification, nci::getState());
    singleShotTimer::mockMillis += (nci::standardResponseTimeout + 1);
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
    nci::handleUnexpectedMessage();
    nci::handleNoResponseTimeout();
    
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_getMessageType);
    RUN_TEST(test_getGroupIdentifier);
    RUN_TEST(test_getOpcodeIdentifier);
    RUN_TEST(test_getMessageId);
    RUN_TEST(test_initialize);
    RUN_TEST(test_reset);
    RUN_TEST(test_check_status);
    RUN_TEST(test_state_machine);
    RUN_TEST(test_state_machine_timeouts);
    RUN_TEST(test_dummy);
    UNITY_END();
}
