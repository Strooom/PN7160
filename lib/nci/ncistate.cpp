#include <ncistate.hpp>

const char* toString(nciState state) {
    switch (state) {
        case nciState::boot:
            return "boot";
        case nciState::venResetActive:
            return "venResetActive";
        case nciState::waitForResetDone:
            return "waitForResetDone";
        case nciState::waitForCoreResetResponse:
            return "waitForCoreResetResponse";
        case nciState::waitForCoreResetNotification:
            return "waitForCoreResetNotification";
        case nciState::waitforCoreInitResponse:
            return "waitforCoreInitResponse";
        case nciState::waitForConfigResponse:
            return "waitForConfigResponse";
        case nciState::waitForDiscoverResponse:
            return "waitForDiscoverResponse";
        case nciState::waitForDiscoverNotification:
            return "waitForDiscoverNotification";
        case nciState::waitForRfDeactivationResponse:
            return "waitForRfDeactivationResponse";
        case nciState::waitForRfDeactivationNotification:
            return "waitForRfDeactivationNotification";
        case nciState::waitForRestartDiscovery:
            return "waitForRestartDiscovery";
        case nciState::error:
            return "error";
        case nciState::test:
            return "Test";
        default:
            return "unknown nciState";
    }
}