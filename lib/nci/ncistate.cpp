#include <ncistate.hpp>

const char* toString(nciState state) {
    switch (state) {
        case nciState::boot:
            return "boot";
        case nciState::venResetActive:
            return "venResetActive";
        case nciState::waitForHwReset:
            return "waitForHwReset";
        case nciState::waitForCoreResetResponse:
            return "waitForCoreResetResponse";
        case nciState::waitForCoreResetNotification:
            return "waitForCoreResetNotification";
        case nciState::waitForCoreInitResponse:
            return "waitForCoreInitResponse";
        case nciState::waitForGetConfigResponse:
            return "waitForGetConfigResponse";
        case nciState::waitForSetConfigResponse:
            return "waitForSetConfigResponse";
        case nciState::waitForDiscoverResponse:
            return "waitForDiscoverResponse";
        case nciState::waitForRfInterfaceActivatedNotification:
            return "waitForRfInterfaceActivatedNotification";
        case nciState::waitForRfDeactivateResponse:
            return "waitForRfDeactivateResponse";
        case nciState::waitForRfDeactivateNotification:
            return "waitForRfDeactivateNotification";
        case nciState::waitForRestartDiscovery:
            return "waitForRestartDiscovery";
        case nciState::error:
            return "error";
        case nciState::test:
            return "test";
        default:
            return "unknown nciState";
    }
}