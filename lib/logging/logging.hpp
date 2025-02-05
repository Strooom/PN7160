// ######################################################################################
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################

#pragma once
#include <stdint.h>
#include <stdarg.h>        // required so we can define functions with variable number of arguments

class logging {
  public:
    logging() = delete;
    static void initialize();
    enum class destination : uint32_t {        // We can send logging to the following destinations, when they are connected and enabled :
        none,
        destUart1
    };
    enum class source : uint32_t {        // We can send logging from the following sources, when they are enabled :
        stateChanges       = 0,
        nciMessages        = 1,
        nciMessageBytes    = 2,
        nciMessagesDecoded = 3,
        tagEvents          = 10,
        error              = 30,
        criticalError      = 31
    };

    static constexpr uint32_t bufferLength{256};
    static void reset() {
        activeSources      = 0;
        activeDestinations = 0;
    }
    static uint32_t snprintf(const char *format, ...);                        // logs always
    static uint32_t snprintf(source aSource, const char *format, ...);        // logs only if the source is active
    static void enable(source aSource) { activeSources = activeSources | (0x01 << static_cast<uint32_t>(aSource)); }
    static void disable(source aSource) { activeSources = activeSources & ~(0x01 << static_cast<uint32_t>(aSource)); }
    static void enable(destination aDestination) { activeDestinations = activeDestinations | (0x01 << static_cast<uint32_t>(aDestination)); }
    static void disable(destination aDestination) { activeDestinations = activeDestinations & ~(0x01 << static_cast<uint32_t>(aDestination)); }
    static bool isActive(source aSource) {
        uint32_t mask = 0x01 << static_cast<uint32_t>(aSource);
        return ((activeSources & mask) != 0);
    }
    static bool isActive(destination aDestination) {
        uint32_t mask = 0x01 << static_cast<uint32_t>(aDestination);
        return ((activeDestinations & mask) != 0);
    }
    static bool isActive() {
        return (activeDestinations != 0);
    }
    static uint32_t getActiveSources() { return activeSources; }
    static void setActiveSources(uint32_t aSources) { activeSources = aSources; }
    static uint32_t getActiveDestinations() { return activeDestinations; }
    static void setActiveDestinations(uint32_t aDestinations) { activeDestinations = aDestinations; }

    static void dump();

#ifndef unitTesting
  private:
#endif
    static char buffer[bufferLength];          // in this buffer the snprintf is expanded before the contents is being sent to one or more of the destinations
    static uint32_t activeSources;             // bitfield of active logging sources
    static uint32_t activeDestinations;        // bitfield of active logging destinations
    static void write(uint32_t dataLength);
};

const char *toString(logging::source aSource);
const char *toString(logging::destination aDestination);