// #############################################################################
// ###                                                                       ###
// ### NXP PN7150 Driver                                                     ###
// ###                                                                       ###
// ### https://github.com/Strooom/PN7150                                     ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include "tag.hpp"
#include "logging.hpp"

const char* toString(tagStatus status) {
    switch (status) {
        case tagStatus::absent:
            return "no tag";
        case tagStatus::foundNew:
            return "new tag";
        case tagStatus::old:
            return "old tag";
        case tagStatus::removed:
            return "tag removed";
        default:
            return "unknown";
    }
}

uint8_t tag::getUniqueIdLength() const {
    return uniqueIdLength;
}

const uint8_t* tag::getUniqueId() const {
    return uniqueId;
}

void tag::setUniqueId(const uint8_t length, const uint8_t* data) {
    switch (length) {
        case 4:
        case 7:
        case 10:
            uniqueIdLength = length;
            break;

        default:
            uniqueIdLength = 0;
            break;
    }
    for (uint8_t index = 0; index < uniqueIdLength; index++) {
        uniqueId[index] = data[index];
    }
}

void tag::dump() const {
    logging::snprintf("uniqueID[%d] = ", uniqueIdLength);
    for (uint8_t index = 0; index < uniqueIdLength; index++) {
        logging::snprintf("0x%02X ", uniqueId[index]);
    }
    logging::snprintf("\n");
}
