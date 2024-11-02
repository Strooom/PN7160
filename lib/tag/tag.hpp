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

#include <stdint.h>

enum class tagStatus : uint8_t {
    noTag,
    newTag,
    oldTag,
    removed
};

const char* toString(tagStatus status);

class tag {
  public:
    uint8_t getUniqueIdLength() const;
    const uint8_t* getUniqueId() const;
    void setUniqueId(const uint8_t length, const uint8_t* data);

    bool operator==(const tag& otherTag) const;
    void dump() const;

    static constexpr uint8_t maxUniqueIdLength{10U};

#ifndef unitTesting
  private:
#endif
    uint8_t uniqueIdLength{0};
    uint8_t uniqueId[maxUniqueIdLength]{0};
};
