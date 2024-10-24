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

#include <stdint.h>        // Gives us access to uint8_t types etc

class tag {
  public:
    tag();
    uint8_t getUniqueIdLength() const;
    void clear();
    bool operator==(const tag& otherTag) const;
    const uint8_t& operator[](int) const;
    void dump() const;

    static constexpr uint8_t maxUniqueIdLength{10U};
    static constexpr uint8_t defaultIdByte{0};

#ifndef unitTesting
  private:
#endif
    uint8_t uniqueIdLength{0};        // Can be 4, 7 or 10 bytes
    uint8_t uniqueId[maxUniqueIdLength]{0};
    unsigned long detectionTimestamp{0};        // TODO : Do we need this ?
};
