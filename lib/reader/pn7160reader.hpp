#pragma once
// Cooperative UID facade over the existing PN7160 NCI state machine.
// The transport/engine are singleton-based: use one reader instance per MCU.
// License: CC BY-NC-SA 4.0, consistent with the parent project.
#include <cstdint>
#include <vector>

class TwoWire;

class PN7160Reader {
public:
    struct Config {
        TwoWire* wire = nullptr; // ESP32 shared bus; RP2040 uses the existing Wire.
        int irq = -1, ven = -1, dwl = -1, sda = -1, scl = -1;
        uint8_t address = 0x28;
        uint32_t clock = 400000;
    };

    explicit PN7160Reader(const Config& value) : config(value) {}
    ~PN7160Reader() { stop(); }
    PN7160Reader(const PN7160Reader&) = delete;
    PN7160Reader& operator=(const PN7160Reader&) = delete;

    bool init();
    void stop();
    bool beginDiscovery();
    bool pollForTag(std::vector<uint8_t>& uid);
    bool isTagStillPresent();
    void releaseTag() {} // NCI automatically deactivates/restarts discovery.
    bool healthCheck() const;
    bool discoveryReady() const { return ready && healthCheck(); }
    bool isConnected() const { return initialized; }

private:
    void pump();
    Config config;
    bool initialized = false, ready = false, present = false, pending = false;
    std::vector<uint8_t> pendingUid;
};
