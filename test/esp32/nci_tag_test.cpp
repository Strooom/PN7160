#include "nci/nci.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <algorithm>
#include <cassert>
#include <cstdio>

static void deliver(TwoWire &bus, const std::vector<uint8_t> &message)
{
    bus.responses.emplace_back(message.begin(), message.begin() + 3);
    if (message.size() > 3)
        bus.responses.emplace_back(message.begin() + 3, message.end());
    pinValues[3] = HIGH;
    nci::run();
    pinValues[3] = LOW;
}

int main()
{
    TwoWire bus;
    assert(PN7160Interface::initialize(bus, 3, 4, 0x28, 5, 6));
    nci::reset();
    nci::run();
    assert(nci::getState() == nciState::venResetActive && pinValues[4] == LOW);
    delayMs += 4; nci::run();
    assert(nci::getState() == nciState::waitForHwReset && pinValues[4] == HIGH);
    delayMs += 4; nci::run();
    assert(nci::getState() == nciState::waitForCoreResetResponse);
    assert(bus.transmitted == std::vector<uint8_t>({0x20, 0x00, 0x01, 0x00}));
    deliver(bus, {0x40, 0x00, 0x01, 0x00});
    assert(nci::getState() == nciState::waitForCoreResetNotification);

    // Test the shared discovery/tag engine with RF activation packets delivered
    // through the actual ESP32 transport, including every legacy UID size.
    for (uint8_t length : {4, 7, 10})
    {
        nci::state = nciState::waitForRfInterfaceActivatedNotification;
        std::vector<uint8_t> activation(14 + length, 0);
        activation[0] = 0x61; activation[1] = 0x05;
        activation[2] = activation.size() - 3; activation[12] = length; activation[9] = 4 + length;
        for (uint8_t i = 0; i < length; ++i) activation[13 + i] = i + length;
        deliver(bus, activation);
        assert(nci::getState() == nciState::waitForRfDeactivateResponse);
        assert(nci::getTagStatus() == tagStatus::foundNew);
        assert(nci::tagData.getUniqueIdLength() == length);
        assert(std::equal(activation.begin() + 13, activation.begin() + 13 + length, nci::tagData.getUniqueId()));
        assert(bus.transmitted == std::vector<uint8_t>({0x21, 0x06, 0x01, 0x00}));
        nci::state = nciState::waitForRfInterfaceActivatedNotification;
        deliver(bus, activation);
        assert(nci::getTagStatus() == tagStatus::old);
        nci::state = nciState::waitForRfInterfaceActivatedNotification;
        nci::noTagFoundTimeoutTimer.start(1);
        delayMs += 2; nci::run();
        assert(nci::getTagStatus() == tagStatus::removed);
        assert(nci::getTagStatus() == tagStatus::absent);
    }
    nci::state = nciState::waitForRfInterfaceActivatedNotification;
    bus.responses = {{0x61, 0x05, 4}, {0x01}};
    pinValues[3] = HIGH; nci::run();
    assert(nci::getState() == nciState::error);
    assert(nci::getTagStatus() != tagStatus::foundNew);
    std::puts("PASS: shared NCI reset, 4/7/10-byte UIDs, repeat suppression, removal and partial-read rejection on ESP32.");
}
