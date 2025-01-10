// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################
#include <unity.h>
#include <ncistate.hpp>

void setUp(void) {        // before each test
}
void tearDown(void) {        // after each test
}

void test_dummy() {
    TEST_ASSERT_EQUAL_STRING("boot", toString(nciState::boot));
    TEST_ASSERT_EQUAL_STRING("venResetActive", toString(nciState::venResetActive));
    TEST_ASSERT_EQUAL_STRING("waitForHwReset", toString(nciState::waitForHwReset));
    TEST_ASSERT_EQUAL_STRING("waitForCoreResetResponse", toString(nciState::waitForCoreResetResponse));
    TEST_ASSERT_EQUAL_STRING("waitForCoreResetNotification", toString(nciState::waitForCoreResetNotification));
    TEST_ASSERT_EQUAL_STRING("waitForCoreInitResponse", toString(nciState::waitForCoreInitResponse));
    TEST_ASSERT_EQUAL_STRING("waitForGetConfigResponse", toString(nciState::waitForGetConfigResponse));
    TEST_ASSERT_EQUAL_STRING("waitForSetConfigResponse", toString(nciState::waitForSetConfigResponse));
    TEST_ASSERT_EQUAL_STRING("waitForDiscoverResponse", toString(nciState::waitForDiscoverResponse));
    TEST_ASSERT_EQUAL_STRING("waitForRfInterfaceActivatedNotification", toString(nciState::waitForRfInterfaceActivatedNotification));
    TEST_ASSERT_EQUAL_STRING("waitForRestartDiscovery", toString(nciState::waitForRestartDiscovery));
    TEST_ASSERT_EQUAL_STRING("error", toString(nciState::error));
    TEST_ASSERT_EQUAL_STRING("test", toString(nciState::test));
    TEST_ASSERT_EQUAL_STRING("unknown nciState", toString(static_cast<nciState>(0xFF)));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy);
    UNITY_END();
}
