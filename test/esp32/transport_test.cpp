#include "pn7160interface/pn7160interface.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>

int main()
{
    TwoWire bus;
    uint8_t packet[259] = {};
    assert(!PN7160Interface::hasMessage());
    assert(!PN7160Interface::wakeUp());
    assert(PN7160Interface::write(packet, 3) == 4);
    assert(!PN7160Interface::initialize(bus, -1, 4, 0x28, 5, 6));
    assert(!PN7160Interface::initialize(bus, 3, -1, 0x28, 5, 6));
    assert(!PN7160Interface::initialize(bus, 3, 4, 0x00, 5, 6));
    assert(!PN7160Interface::initialize(bus, 3, 4, 0x78, 5, 6));
    assert(gpioCalls == 0);
    bus.bufferResult = false;
    assert(!PN7160Interface::initialize(bus, 3, 4, 0x28, 5, 6));
    bus.bufferResult = true; bus.beginResult = false;
    assert(!PN7160Interface::initialize(bus, 3, 4, 0x28, 5, 6));
    assert(!PN7160Interface::wakeUp());
    bus.beginResult = true;
    assert(PN7160Interface::initialize(bus, 3, 4, 0x28, 5, 6));
    assert(bus.bufferSize == 258 && bus.timeout == 20);
    assert(bus.sda == 5 && bus.scl == 6 && bus.clock == 400000);
    assert(pinValues[4] == HIGH);
    PN7160Interface::setVenPin(false); assert(pinValues[4] == LOW);
    PN7160Interface::setVenPin(true); assert(pinValues[4] == HIGH);
    assert(!PN7160Interface::hasMessage());
    pinValues[3] = HIGH;
    assert(PN7160Interface::hasMessage());

    bus.status = {2, 2, 0};
    assert(PN7160Interface::wakeUp());
    assert(bus.transmissions == 3 && delayMs == 10);
    bus.status = {2, 2, 2};
    assert(!PN7160Interface::wakeUp());
    assert(bus.transmissions == 6 && delayMs == 25);
    assert(Wire.transmissions == 0); // The injected bus, not the global Wire, is used.

    std::array<uint8_t, 258> maximum{};
    maximum[2] = 255;
    assert(PN7160Interface::write(maximum.data(), maximum.size()) == 0);
    assert(bus.transmitted.size() == 258);
    bus.shortWrite = true;
    assert(PN7160Interface::write(maximum.data(), maximum.size()) == 4);
    bus.shortWrite = false; bus.status = {2};
    assert(PN7160Interface::write(maximum.data(), maximum.size()) == 4);
    int before = bus.transmissions;
    assert(PN7160Interface::write(nullptr, 3) == 4);
    assert(PN7160Interface::write(packet, 0) == 4);
    assert(PN7160Interface::write(packet, 259) == 4);
    assert(before == bus.transmissions);

    bus.responses = {{0x40, 0x01, 0}};
    assert(PN7160Interface::read(packet) == 3 && packet[0] == 0x40);
    std::vector<uint8_t> payload(255);
    for (size_t i = 0; i < payload.size(); ++i) payload[i] = static_cast<uint8_t>(i);
    bus.responses = {{0x61, 0x05, 255}, payload};
    packet[258] = 0xA5;
    assert(PN7160Interface::read(packet) == 258);
    assert(packet[258] == 0xA5 && packet[2] == 255);
    assert(std::equal(payload.begin(), payload.end(), packet + 3));
    assert(bus.lastAddress == 0x28);

    // Truncation/timeout must not expose the previous good response to NCI.
    bus.responses = {{0x40, 0x01}};
    assert(PN7160Interface::read(packet) == 0 && packet[0] == 0xFF && packet[2] == 0);
    bus.responses = {{0x61, 0x05, 4}, {1, 2}};
    assert(PN7160Interface::read(packet) == 0 && packet[0] == 0xFF);
    bus.responses = {{0x40, 0x01, 0}}; bus.unreadableAt = 1;
    assert(PN7160Interface::read(packet) == 0 && packet[0] == 0xFF);
    bus.unreadableAt = -1; bus.responses.clear();
    assert(PN7160Interface::read(packet) == 0);
    pinValues[3] = LOW;
    assert(PN7160Interface::read(packet) == 0);
    assert(PN7160Interface::read(nullptr) == 0);
    // Existing initialization calls continue to select the global Wire bus.
    PN7160Interface::initialize(3, 4, 0x29);
    assert(Wire.bufferSize == 258 && Wire.sda == -1 && Wire.scl == -1);
    assert(PN7160Interface::write(maximum.data(), maximum.size()) == 0);
    assert(Wire.lastAddress == 0x29 && Wire.transmissions == 1);
    std::puts("PASS: ESP32 PN7160 init, bus selection, IRQ/VEN, retries, full NCI packets and I2C failures.");
}
