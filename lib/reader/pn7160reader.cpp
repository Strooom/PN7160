// License: CC BY-NC-SA 4.0, consistent with the parent project.
#if defined(ARDUINO) || defined(ARDUINO_ARCH_ESP32) || defined(PN7160_RP2040)
#include "pn7160reader.hpp"
#include "../nci/nci.hpp"
#include <Arduino.h>
#ifdef ARDUINO_ARCH_ESP32
#include <Wire.h>
#endif

bool PN7160Reader::init() {
    if (initialized) return true;
    if (config.irq < 0 || config.ven < 0 || config.irq > 127 || config.ven > 127
        || config.address < 8 || config.address > 0x77) return false;
#ifdef ARDUINO_ARCH_ESP32
    if (!config.wire || !PN7160Interface::initialize(*config.wire, config.irq,
        config.ven, config.address, config.sda, config.scl, config.clock)) return false;
#else
    PN7160Interface::initialize(config.irq, config.ven, config.address);
#endif
    if (config.dwl >= 0) {
        pinMode(config.dwl, OUTPUT);
        digitalWrite(config.dwl, LOW);
    }
    nci::reset();
    nci::tagData.setUniqueId(0, nullptr);
    initialized = true;
    ready = present = pending = false;
    pendingUid.clear();
    return true;
}

void PN7160Reader::stop() {
    if (initialized) {
        PN7160Interface::setVenPin(false);
        nci::reset();
        nci::tagData.setUniqueId(0, nullptr);
    }
    initialized = ready = present = pending = false;
    pendingUid.clear();
    // The caller owns Wire, including any other devices sharing it.
}

bool PN7160Reader::beginDiscovery() { return healthCheck(); }

void PN7160Reader::pump() {
    if (!initialized) return;
    nci::run();
    if (nci::getState() == nciState::waitForRfInterfaceActivatedNotification) ready = true;
    const auto status = nci::getTagStatus();
    if (status == tagStatus::removed || status == tagStatus::absent) {
        present = false;
        return;
    }
    if (status != tagStatus::foundNew) return;
    const auto length = nci::tagData.getUniqueIdLength();
    if (length != 4 && length != 7 && length != 10) {
        present = false;
        return;
    }
    pendingUid.assign(nci::tagData.getUniqueId(), nci::tagData.getUniqueId() + length);
    ready = present = pending = true;
}

bool PN7160Reader::pollForTag(std::vector<uint8_t>& uid) {
    uid.clear();
    if (!pending) pump();
    if (!healthCheck()) {
        pending = false;
        return false;
    }
    if (!pending) return false;
    uid = pendingUid;
    pending = false;
    return true;
}

bool PN7160Reader::isTagStillPresent() {
    pump();
    // Preserve a different tag for the next poll while reporting that the
    // previously selected tag is no longer the current presentation.
    return healthCheck() && present && !pending;
}

bool PN7160Reader::healthCheck() const {
    return initialized && nci::getState() != nciState::error;
}
#endif
