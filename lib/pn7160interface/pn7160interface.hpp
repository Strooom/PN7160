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

// Summary :
//   This library implements the hardware interface for the NXP PN7160 NFC device.
//   The PN7160 uses I2C plus 2 additional data signals : IRQ and VEN
//     IRQ : output of the PN7160, input for the MCU. Through this signal the PN7160 signals it has data to be read by the MCU
//     VEN : input of the PN7160, output for the MCU. Through this signal the MCU can RESET the PN7160
//
//   The library provides :
//   * init() : Initialization of the interface
//   * read() : Read message from PN7160 over I2C
//   * write() : Write message to PN7160 over I2C
//   * hasMessage() : Check if PN7160 has message waiting for MCU

#include <stdint.h>                                  // Gives us access to uint8_t types etc.

class PN7160Interface {
  public:
    static void initialize(uint8_t IRQpin, uint8_t VENpin, uint8_t I2Caddress = 0x28);
    static void reset();
    static bool wakeUp();
    static uint8_t write(const uint8_t data[], const uint32_t dataLength) ;
    static uint32_t read(uint8_t data[]) ;
    static bool hasMessage() ;

  private:
    static uint8_t IRQpin;
    static uint8_t VENpin;
    static uint8_t I2Caddress;
    static constexpr uint8_t i2cError{4U};
    static constexpr uint8_t i2cRetries{3U};
};
