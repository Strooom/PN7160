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

#ifndef generic
#include <Arduino.h>
#include <i2c_t3.h>
#else
bool PN7160Interface::mockIrqPin{false};
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
    for (uint8_t tryCounter = 0; tryCounter < i2cRetries; tryCounter++) {
#ifndef generic
        Wire.beginTransmission(I2Caddress);
        if (Wire.endTransmission() == 0) {
            return true;
        }
        delay(i2cRetryTimout);
#endif
    }
    return false;
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
#endif
    if ((nmbrBytesWritten != txBufferLevel) || (resultCode != 0)) {
        return i2cError;
    } else {
        return 0;
    }
}

uint32_t PN7160Interface::read(uint8_t rxBuffer[]) {
    uint32_t bytesReceived{0};
#ifndef generic
    if (hasMessage()) {
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
    } else {
        bytesReceived = 0;
    }
#endif
    return bytesReceived;
}
