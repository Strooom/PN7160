// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <ncipacket.hpp>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_dummy() {
    TEST_ASSERT_EQUAL_STRING("ok", toString(nciStatus::ok));
    TEST_ASSERT_EQUAL_STRING("rejected", toString(nciStatus::rejected));
    TEST_ASSERT_EQUAL_STRING("failed", toString(nciStatus::failed));
    TEST_ASSERT_EQUAL_STRING("invalid parameter", toString(nciStatus::invalidParam));
    TEST_ASSERT_EQUAL_STRING("other statusCode", toString(static_cast<nciStatus>(0xFF)));

    TEST_ASSERT_EQUAL_STRING("CORE_RESET_CMD", toString(nciMessageId::CORE_RESET_CMD));
    TEST_ASSERT_EQUAL_STRING("CORE_RESET_RSP", toString(nciMessageId::CORE_RESET_RSP));
    TEST_ASSERT_EQUAL_STRING("CORE_RESET_NTF", toString(nciMessageId::CORE_RESET_NTF));
    TEST_ASSERT_EQUAL_STRING("CORE_INIT_CMD", toString(nciMessageId::CORE_INIT_CMD));
    TEST_ASSERT_EQUAL_STRING("CORE_INIT_RSP", toString(nciMessageId::CORE_INIT_RSP));
    TEST_ASSERT_EQUAL_STRING("RF_DISCOVER_RSP", toString(nciMessageId::RF_DISCOVER_RSP));
    TEST_ASSERT_EQUAL_STRING("RF_DISCOVER_NTF", toString(nciMessageId::RF_DISCOVER_NTF));
    TEST_ASSERT_EQUAL_STRING("RF_INTF_ACTIVATED_NTF", toString(nciMessageId::RF_INTF_ACTIVATED_NTF));
    TEST_ASSERT_EQUAL_STRING("RF_DEACTIVATE_RSP", toString(nciMessageId::RF_DEACTIVATE_RSP));
    TEST_ASSERT_EQUAL_STRING("Unknown Message", toString(static_cast<nciMessageId>(0xFF)));
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy);
    UNITY_END();
}
