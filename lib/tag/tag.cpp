// #############################################################################
// ###                                                                       ###
// ### NXP PN7150 Driver                                                     ###
// ###                                                                       ###
// ### https://github.com/Strooom/PN7150                                     ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <tag.hpp>
#include <logging.hpp>

tag::tag() {
    clear();
}

void tag::clear() {
    uniqueIdLength = 0;
    for (uint8_t index = 0; index < maxUniqueIdLength; index++) {
        uniqueId[index] = 0;
    }
}

uint8_t tag::getUniqueIdLength() const {
    return uniqueIdLength;
}

const uint8_t& tag::operator[](int index) const {
    if (index < uniqueIdLength) {
        return uniqueId[index];
    }
    return defaultIdByte;
}

bool tag::operator==(const tag& otherTag) const {
    if (uniqueIdLength == otherTag.uniqueIdLength) {
        for (uint8_t index = 0; index < uniqueIdLength; index++) {
            if (uniqueId[index] != otherTag.uniqueId[index]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

void tag::dump() const {
    logging::snprintf("uniqueID[%d] = ", uniqueIdLength);
    for (uint8_t index = 0; index < maxUniqueIdLength; index++) {
        logging::snprintf("0x%02X ", uniqueId[index]);
    }
}