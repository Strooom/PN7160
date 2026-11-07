#include "reader/pn7160reader.hpp"
#include "nci/nci.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <type_traits>

static void packet(const std::vector<uint8_t>& data) {
    Wire.responses.emplace_back(data.begin(), data.begin() + 3);
    if (data.size() > 3) Wire.responses.emplace_back(data.begin() + 3, data.end());
    pinValues[3] = HIGH;
}

static void activation(const std::vector<uint8_t>& uid) {
    nci::state = nciState::waitForRfInterfaceActivatedNotification;
    std::vector<uint8_t> data(14 + uid.size(), 0);
    data[0] = 0x61; data[1] = 0x05; data[2] = data.size() - 3;
    data[12] = uid.size(); data[9] = 4 + uid.size();
    std::copy(uid.begin(), uid.end(), data.begin() + 13);
    packet(data);
}

int main() {
    static_assert(!std::is_copy_constructible_v<PN7160Reader>);
    static_assert(!std::is_move_constructible_v<PN7160Reader>);
    PN7160Reader::Config config;
    config.wire = &Wire; config.irq = 3; config.ven = 4;
    config.sda = 5; config.scl = 6; config.dwl = 8;
    PN7160Reader reader(config);
    std::vector<uint8_t> uid{99};
    assert(!reader.healthCheck() && !reader.beginDiscovery());
    assert(!reader.pollForTag(uid) && uid.empty());
    assert(reader.init() && reader.init() && reader.beginDiscovery());
    assert(!reader.discoveryReady() && pinValues[8] == LOW);
    assert(!reader.pollForTag(uid) && pinValues[4] == LOW);
    delayMs += 4; reader.pollForTag(uid); assert(pinValues[4] == HIGH);
    delayMs += 4; reader.pollForTag(uid);
    assert(Wire.transmitted == std::vector<uint8_t>({0x20, 0, 1, 0}));
    packet({0x40, 0, 1, 0}); reader.pollForTag(uid); pinValues[3] = LOW;
    assert(nci::getState() == nciState::waitForCoreResetNotification);
    nci::state = nciState::waitForDiscoverResponse;
    packet({0x41, 3, 1, 0});
    assert(!reader.pollForTag(uid) && reader.discoveryReady()); pinValues[3] = LOW;
    for (size_t length : {4, 7, 10}) {
        std::vector<uint8_t> expected(length, static_cast<uint8_t>(length));
        activation(expected);
        assert(reader.pollForTag(uid) && uid == expected);
        pinValues[3] = LOW; assert(reader.isTagStillPresent());
        activation(expected);
        assert(!reader.pollForTag(uid)); pinValues[3] = LOW;
        expected[0] ^= 0x10; activation(expected);
        assert(!reader.isTagStillPresent()); pinValues[3] = LOW;
        assert(reader.pollForTag(uid) && uid == expected);
        nci::state = nciState::waitForRfInterfaceActivatedNotification;
        nci::noTagFoundTimeoutTimer.start(1); delayMs += 2;
        assert(!reader.isTagStillPresent());
    }
    nci::state = nciState::error;
    assert(!reader.pollForTag(uid) && uid.empty());
    assert(!reader.healthCheck() && !reader.isTagStillPresent());
    assert(!reader.discoveryReady() && !reader.beginDiscovery());
    reader.stop(); reader.stop();
    assert(!reader.isConnected() && !reader.discoveryReady());
    assert(pinValues[4] == LOW && nci::tagData.getUniqueIdLength() == 0);
    assert(reader.init());
    activation({1, 2, 3, 4}); assert(reader.pollForTag(uid)); pinValues[3] = LOW;
    reader.releaseTag(); reader.stop();
    auto invalid = config; invalid.irq = -1;
    PN7160Reader absent(invalid);
    const auto calls = gpioCalls;
    assert(!absent.init() && gpioCalls == calls);
    std::puts("Cooperative reader lifecycle, UIDs, replacement tags and errors passed");
}
