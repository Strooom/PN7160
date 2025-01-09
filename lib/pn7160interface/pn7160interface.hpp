#pragma once
// #############################################################################
// ###                                                                       ###
// ### NXP PN7160 Driver                                                     ###
// ###                                                                       ###
// ### https://github.com/Strooom/PN7160                                     ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

//   The PN7160 uses I2C plus 2 additional data signals : IRQ and VEN
//     IRQ : output of the PN7160, input for the MCU. Through this signal the PN7160 signals it has data to be read by the MCU, active HIGH
//     VEN : input of the PN7160, output for the MCU. Through this signal the MCU can RESET the PN7160, active LOW

#include <stdint.h>

class PN7160Interface {
  public:
    static void initialize(uint8_t IRQpin, uint8_t VENpin, uint8_t I2Caddress = 0x28);
    static void setVenPin(bool highOrLow);
    static bool wakeUp();
    static uint8_t write(const uint8_t data[], const uint32_t dataLength);
    static uint32_t read(uint8_t data[]);
    static bool hasMessage();

#ifndef unitTesting
  private:
#endif
    static uint8_t IRQpin;
    static uint8_t VENpin;
    static uint8_t I2Caddress;
    static constexpr uint8_t i2cError{4U};
    static constexpr uint8_t i2cRetries{3U};
    static constexpr uint8_t i2cRetryTimout{5U};
#ifdef generic
    static bool mockIrqPin;
    static bool mockWakeUp;
    static uint8_t mockMessageData[255];
#endif
};
