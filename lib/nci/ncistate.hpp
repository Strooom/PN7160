#pragma once

#include <stdint.h>

enum class nciState : uint8_t {
    boot,
    venResetActive,

    waitForHwReset,
    waitForCoreResetResponse,
    waitForCoreResetNotification,

    waitForCoreInitResponse,

    waitForGetConfigResponse,
    waitForSetConfigResponse,

    waitForDiscoverResponse,
    waitForRfInterfaceActivatedNotification,

    waitForRfDeactivateResponse,
    waitForRfDeactivateNotification,

    waitForRestartDiscovery,

    error,
    test

};

const char* toString(nciState state);