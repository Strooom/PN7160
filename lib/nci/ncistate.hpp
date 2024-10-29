#pragma once

#include <stdint.h>

enum class nciState : uint8_t {
    boot,
    venResetActive,

    waitForResetDone,
    waitForCoreResetResponse,
    waitForCoreResetNotification,

    waitforCoreInitResponse,

    waitForConfigResponse,

    waitForDiscoverResponse,
    waitForDiscoverNotification,

    waitForRfDeactivationResponse,
    waitForRfDeactivationNotification,

    error,
    test

};

const char* toString(nciState state);