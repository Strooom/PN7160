// #############################################################################
// ###                                                                       ###
// ### NXP PN7160 Driver                                                     ###
// ###                                                                       ###
// ### https://github.com/Strooom/PN7160                                     ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include "pn7160interface.hpp"

#if defined(ARDUINO_ARCH_ESP32) && !defined(generic)
#include <Arduino.h>
#include <Wire.h>
#include <cstring>

TwoWire *PN7160Interface::espBus{nullptr};
bool PN7160Interface::espReady{false};
uint8_t PN7160Interface::IRQpin{};
uint8_t PN7160Interface::VENpin{};
uint8_t PN7160Interface::I2Caddress{0x28};

bool PN7160Interface::initialize(TwoWire &bus, int8_t irq, int8_t ven, uint8_t address,
                                int8_t sda, int8_t scl, uint32_t clock) {
    espReady = false;
    espBus = nullptr;
    if (irq < 0 || ven < 0 || address < 0x08 || address > 0x77) {
        return false;
    }
    // The NCI engine sends/receives a 3-byte header plus up to 255 payload bytes.
    if (bus.setBufferSize(258) < 258 || !bus.begin(sda, scl, clock)) {
        return false;
    }
    bus.setTimeOut(20);
    IRQpin = static_cast<uint8_t>(irq);
    VENpin = static_cast<uint8_t>(ven);
    I2Caddress = address;
    espBus = &bus;
    pinMode(IRQpin, INPUT);
    pinMode(VENpin, OUTPUT);
    digitalWrite(VENpin, HIGH);
    espReady = true;
    return true;
}

void PN7160Interface::initialize(uint8_t irq, uint8_t ven, uint8_t address) {
    initialize(Wire, static_cast<int8_t>(irq), static_cast<int8_t>(ven), address);
}

void PN7160Interface::setVenPin(bool highOrLow) {
    if (espReady) {
        digitalWrite(VENpin, highOrLow ? HIGH : LOW);
    }
}

bool PN7160Interface::wakeUp() {
    if (!espReady) {
        return false;
    }
    for (uint8_t attempt = 0; attempt < i2cRetries; ++attempt) {
        espBus->beginTransmission(I2Caddress);
        if (espBus->endTransmission() == 0) {
            return true;
        }
        delay(i2cRetryTimout);
    }
    return false;
}

bool PN7160Interface::hasMessage() {
    return espReady && digitalRead(IRQpin) == HIGH;
}

uint8_t PN7160Interface::write(const uint8_t data[], uint32_t length) {
    if (!espReady || !data || length == 0 || length > 258) {
        return i2cError;
    }
    espBus->beginTransmission(I2Caddress);
    const size_t written = espBus->write(data, length);
    const uint8_t status = espBus->endTransmission();
    return written == length && status == 0 ? 0 : i2cError;
}

uint32_t PN7160Interface::read(uint8_t data[]) {
    if (!data) {
        return 0;
    }
    // A failed read must never leave a previous, valid response for NCI to parse.
    data[0] = 0xFF;
    data[1] = 0xFF;
    data[2] = 0;
    if (!hasMessage()) {
        return 0;
    }
    uint8_t header[3];
    if (espBus->requestFrom(I2Caddress, size_t{3}) != 3) {
        return 0;
    }
    for (uint8_t index = 0; index < 3; ++index) {
        const int value = espBus->read();
        if (value < 0) {
            return 0;
        }
        header[index] = static_cast<uint8_t>(value);
    }
    const size_t length = header[2];
    if (length > 0) {
        if (espBus->requestFrom(I2Caddress, length) != length) {
            return 0;
        }
        for (size_t index = 0; index < length; ++index) {
            const int value = espBus->read();
            if (value < 0) {
                return 0;
            }
            data[3 + index] = static_cast<uint8_t>(value);
        }
    }
    std::memcpy(data, header, sizeof(header));
    return sizeof(header) + length;
}

#else


#ifndef generic
#include <Arduino.h>

#ifdef PN7160_RP2040
#include <Wire.h>
#else
#include <i2c_t3.h>
#endif

#else
#include <cstring>
bool PN7160Interface::mockIrqPin{false};
bool PN7160Interface::mockWakeUp{true};
uint8_t PN7160Interface::mockMessageData[255]{};

#endif

uint8_t PN7160Interface::IRQpin{};
uint8_t PN7160Interface::VENpin{};
uint8_t PN7160Interface::I2Caddress{0x28};

void PN7160Interface::initialize(uint8_t theIRQpin, uint8_t theVENpin, uint8_t theI2Caddress) {
    IRQpin     = theIRQpin;
    VENpin     = theVENpin;
    I2Caddress = theI2Caddress;
#ifndef generic
    pinMode(IRQpin, INPUT);
    pinMode(VENpin, OUTPUT);
    digitalWrite(VENpin, HIGH);
    Wire.begin();
#endif
}

void PN7160Interface::setVenPin(bool highOrLow) {
#ifndef generic
    uint8_t pinState = highOrLow ? HIGH : LOW;
    digitalWrite(VENpin, pinState);
#endif
}

bool PN7160Interface::wakeUp() {
#ifndef generic
    for (uint8_t tryCounter = 0; tryCounter < i2cRetries; tryCounter++) {
        Wire.beginTransmission(I2Caddress);
        if (Wire.endTransmission() == 0) {
            return true;
        }
        delay(i2cRetryTimout);
    }
    return false;
#else
    return mockWakeUp;
#endif
}

bool PN7160Interface::hasMessage() {
#ifndef generic
    return (HIGH == digitalRead(IRQpin));
#else
    return mockIrqPin;
#endif
}

uint8_t PN7160Interface::write(const uint8_t txBuffer[], const uint32_t txBufferLevel) {
    uint8_t resultCode{0};
    uint32_t nmbrBytesWritten{0};
#ifndef generic
    Wire.beginTransmission(I2Caddress);
    nmbrBytesWritten = Wire.write(txBuffer, txBufferLevel);
    resultCode       = Wire.endTransmission();
#else
    memcpy(mockMessageData, txBuffer, txBufferLevel);
    nmbrBytesWritten = txBufferLevel;
#endif
    if ((nmbrBytesWritten != txBufferLevel) || (resultCode != 0)) {
        return i2cError;
    } else {
        return 0;
    }
}

uint32_t PN7160Interface::read(uint8_t rxBuffer[]) {
    uint32_t bytesReceived{0};
    if (hasMessage()) {
#ifndef generic
        bytesReceived         = Wire.requestFrom((int)I2Caddress, 3);        // first reading the header, as this contains how long the payload will be
        rxBuffer[0]           = static_cast<uint8_t>(Wire.read());
        rxBuffer[1]           = static_cast<uint8_t>(Wire.read());
        rxBuffer[2]           = static_cast<uint8_t>(Wire.read());
        uint8_t payloadLength = rxBuffer[2];
        if (payloadLength > 0) {
            bytesReceived += Wire.requestFrom(I2Caddress, payloadLength);        // then reading the payload, if any
            uint32_t index = 3;
            while (index < bytesReceived) {
                rxBuffer[index] = static_cast<uint8_t>(Wire.read());
                index++;
            }
        }
#else
        bytesReceived = 3 + mockMessageData[2];
        memcpy(rxBuffer, mockMessageData, bytesReceived);
#endif
    }
    return bytesReceived;
}

#endif
