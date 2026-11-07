// License: CC BY-NC-SA 4.0, consistent with the parent project.
#include "nci.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>

static std::vector<uint8_t> activation(uint8_t uidLength, uint8_t selLength = 1) {
    const uint8_t technologyLength = 4 + uidLength + selLength;
    // Header, seven fixed activation fields, NFC-A poll parameters, then the
    // data-exchange mode, TX/RX bitrates and activation-parameters length.
    std::vector<uint8_t> packet(10 + technologyLength + 4, 0);
    packet[0] = 0x61; packet[1] = 0x05; packet[2] = packet.size() - 3;
    packet[4] = 0x01; packet[5] = 0x02; packet[6] = 0x00;
    packet[9] = technologyLength;
    packet[10] = 0x44; packet[11] = 0x00; packet[12] = uidLength;
    for (uint8_t i = 0; i < uidLength; ++i) packet[13 + i] = 0xA0 + i;
    packet[13 + uidLength] = selLength;
    if (selLength) packet[14 + uidLength] = 0x20;
    return packet;
}

static void deliver(const std::vector<uint8_t>& packet) {
    // Preserve the receive-buffer tail, exactly as it is after shorter reads.
    assert(packet.size() <= sizeof(nci::rxBuffer));
    std::copy(packet.begin(), packet.end(), nci::rxBuffer);
    nci::rxMessageLength = packet.size();
    nci::state = nciState::waitForRfInterfaceActivatedNotification;
    nci::handleRfInterfaceActivatedNotification();
}

static void reject(const std::vector<uint8_t>& malformed) {
    const auto seed = activation(7);
    deliver(seed);
    assert(nci::getState() == nciState::waitForRfDeactivateResponse);
    assert(nci::tagData.getUniqueIdLength() == 7);
    deliver(malformed);
    assert(nci::getState() == nciState::error);
    assert(nci::getTagStatus() == tagStatus::absent);
    assert(nci::tagData.getUniqueIdLength() == 7);
    assert(std::equal(seed.begin() + 13, seed.begin() + 20, nci::tagData.getUniqueId()));
}

int main() {
    for (uint8_t uidLength : {4, 7, 10}) {
        for (uint8_t selLength : {0, 1}) {
            auto packet = activation(uidLength, selLength);
            // Force a different previous UID to exercise the new-tag path.
            nci::tagData.setUniqueId(0, nullptr);
            deliver(packet);
            assert(nci::getState() == nciState::waitForRfDeactivateResponse);
            assert(nci::getTagStatus() == tagStatus::foundNew);
            assert(nci::tagData.getUniqueIdLength() == uidLength);
            assert(std::equal(packet.begin() + 13, packet.begin() + 13 + uidLength,
                              nci::tagData.getUniqueId()));
            deliver(packet);
            assert(nci::getTagStatus() == tagStatus::old);

            const size_t technologyEnd = 10 + packet[9];
            for (size_t size = 0; size < technologyEnd; ++size) {
                auto prefix = packet;
                prefix.resize(size);
                if (size >= 3) prefix[2] = size - 3;
                reject(prefix);
            }
        }
    }

    auto valid = activation(7);
    for (uint8_t uidLength : {0, 1, 3, 5, 6, 8, 9, 11, 255}) {
        auto malformed = valid;
        malformed[12] = uidLength;
        reject(malformed);
    }
    for (uint8_t technologyLength : {0, 3, 10, 255}) {
        auto malformed = valid;
        malformed[9] = technologyLength;
        reject(malformed);
    }
    for (uint8_t offset : {2, 6, 20}) {
        auto malformed = valid;
        malformed[offset] = 255;
        reject(malformed);
    }
    auto missingSak = valid;
    missingSak[9] = 11; // SEL_RES length still declares a byte beyond this field.
    reject(missingSak);
    std::puts("Activation bounds, stale-buffer rejection and valid UID handling passed");
}
