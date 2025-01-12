// ######################################################################################
// ### Author : Pascal Roobrouck  https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA  https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include "logging.hpp"
#ifndef generic
#include <Arduino.h>
#endif

uint32_t logging::activeSources{0};
uint32_t logging::activeDestinations{0};
char logging::buffer[bufferLength]{};

void logging::initialize() {
#ifndef generic
    Serial.begin(115200);
#endif
}

uint32_t logging::snprintf(const char *format, ...) {
    uint32_t length{0};
    if (activeDestinations != 0) {
        va_list argList;
        va_start(argList, format);
        length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);
        write(length);
    }
    return length;
}

uint32_t logging::snprintf(source aSource, const char *format, ...) {
    uint32_t length{0};
    if ((activeDestinations != 0) && (isActive(aSource))) {
        va_list argList;
        va_start(argList, format);
        length = vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);
        write(length);
    }
    return length;
}

void logging::write(uint32_t dataLength) {
    if (isActive(destination::uart1)) {
#ifndef generic
        Serial.write(buffer, dataLength);
#endif
    }
}

void logging::dump() {
    logging::snprintf("logging : \n");
    if (activeSources == 0) {
        logging::snprintf("  no active sources\n");
    } else {
        logging::snprintf("  sources      : ");
        for (uint32_t index = 0; index < 32; index++) {
            if ((activeSources & (0x01 << index)) != 0) {
                logging::snprintf("%s ", toString(static_cast<source>(index)));
            }
        }
        logging::snprintf("\n");
    }
    logging::snprintf("  destinations : ");
    for (uint32_t index = 0; index < 32; index++) {
        if ((activeDestinations & (0x01 << index)) != 0) {
            logging::snprintf("%s ", toString(static_cast<destination>(index)));
        }
    }
    logging::snprintf("\n");
}
const char *toString(logging::source aSource) {
    switch (aSource) {
        case logging::source::stateChanges:
            return "stateChanges";
        case logging::source::nciMessages:
            return "nciMessages";
        case logging::source::tagEvents:
            return "tagEvents";
        case logging::source::error:
            return "error";
        case logging::source::criticalError:
            return "criticalError";
        default:
            return "unknown";
    }
}

const char *toString(logging::destination aDestination) {
    switch (aDestination) {
        case logging::destination::uart1:
            return "uart1";
        default:
            return "unknown";
    }
}
