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
#endif

uint8_t PN7160Interface::IRQpin;
uint8_t PN7160Interface::VENpin;
uint8_t PN7160Interface::I2Caddress{0x28};

void PN7160Interface::initialize(uint8_t theIRQpin, uint8_t theVENpin, uint8_t I2Caddress) {
    IRQpin     = theIRQpin;
    VENpin     = theVENpin;
    I2Caddress = I2Caddress;
#ifndef generic
    pinMode(IRQpin, INPUT);
    pinMode(VENpin, OUTPUT);
    digitalWrite(VENpin, HIGH);
    Wire.begin();
#endif
}

void PN7160Interface::reset() {
#ifndef generic
    digitalWrite(VENpin, LOW);        // PN7160 Reset procedure : see PN7160 datasheet 11.5.1 - Fig 20
    delay(3);
    digitalWrite(VENpin, HIGH);
    delay(3);
#endif
}

bool PN7160Interface::wakeUp() {
    for (uint8_t tryCounter = 0; tryCounter < i2cRetries; tryCounter++) {
#ifndef generic
        Wire.beginTransmission(I2Caddress);
        if (Wire.endTransmission() == 0) {
            return true;
        }
        delay(3U);
#endif
    }
    return false;
}

bool PN7160Interface::hasMessage() {
#ifndef generic
    return (HIGH == digitalRead(IRQpin));
#else
    return true;
#endif
}

uint8_t PN7160Interface::write(const uint8_t txBuffer[], const uint32_t txBufferLevel) {
    Wire.beginTransmission(I2Caddress);
    uint32_t nmbrBytesWritten{0};
    nmbrBytesWritten = Wire.write(txBuffer, txBufferLevel);
    if (nmbrBytesWritten == txBufferLevel) {
        uint8_t resultCode;
        resultCode = Wire.endTransmission();
        return resultCode;
    } else {
        return i2cError;
    }
}

uint32_t PN7160Interface::read(uint8_t rxBuffer[]) {
    uint32_t bytesReceived;
    if (hasMessage()) {
        // using 'Split mode' I2C read. See UM10936 section 3.5
        bytesReceived = Wire.requestFrom((int)I2Caddress, 3);        // first reading the header, as this contains how long the payload will be

        rxBuffer[0]           = Wire.read();
        rxBuffer[1]           = Wire.read();
        rxBuffer[2]           = Wire.read();
        uint8_t payloadLength = rxBuffer[2];
        if (payloadLength > 0) {
            bytesReceived += Wire.requestFrom(I2Caddress, payloadLength);        // then reading the payload, if any
            uint32_t index = 3;
            while (index < bytesReceived) {
                rxBuffer[index] = Wire.read();
                index++;
            }
        }
    } else {
        bytesReceived = 0;
    }
    return bytesReceived;
}
