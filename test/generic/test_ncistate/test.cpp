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
//    TEST_ASSERT_EQUAL_STRING("waitForResetDone", toString(nciState::waitForResetDone));
    TEST_ASSERT_EQUAL_STRING("waitForCoreResetResponse", toString(nciState::waitForCoreResetResponse));
    TEST_ASSERT_EQUAL_STRING("waitForCoreResetNotification", toString(nciState::waitForCoreResetNotification));
    // TEST_ASSERT_EQUAL_STRING("waitforCoreInitResponse", toString(nciState::waitforCoreInitResponse));
    // TEST_ASSERT_EQUAL_STRING("waitForConfigResponse", toString(nciState::waitForConfigResponse));
    TEST_ASSERT_EQUAL_STRING("waitForDiscoverResponse", toString(nciState::waitForDiscoverResponse));
    //TEST_ASSERT_EQUAL_STRING("waitForDiscoverNotification", toString(nciState::waitForDiscoverNotification));
    //TEST_ASSERT_EQUAL_STRING("waitForRfDeactivationResponse", toString(nciState::waitForRfDeactivationResponse));
    //TEST_ASSERT_EQUAL_STRING("waitForRfDeactivationNotification", toString(nciState::waitForRfDeactivationNotification));
    TEST_ASSERT_EQUAL_STRING("waitForRestartDiscovery", toString(nciState::waitForRestartDiscovery));
    TEST_ASSERT_EQUAL_STRING("error", toString(nciState::error));
    TEST_ASSERT_EQUAL_STRING("unknown nciState", toString(static_cast<nciState>(0xFF)));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy);
    UNITY_END();
}
