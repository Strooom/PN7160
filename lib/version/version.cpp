#include "version.hpp"
#include "buildinfo.hpp"
#include <stdio.h>
#include <logging.hpp>

uint8_t version::isVersionMajor{0};
uint8_t version::isVersionMinor{0};
uint8_t version::isVersionPatch{0};
buildType version::theBuildType{buildType::unknown};
buildEnvironment version::theBuildEnvironment{buildEnvironment::unknown};

char version::isVersionString[maxVersionStringLength]{};

void version::initialize() {
    isVersionMajor      = buildInfo::mainVersionDigit;
    isVersionMinor      = buildInfo::minorVersionDigit;
    isVersionPatch      = buildInfo::patchVersionDigit;
    theBuildType        = buildInfo::theBuildType;
    theBuildEnvironment = buildInfo::theBuildEnvironment;
    snprintf(isVersionString, maxVersionStringLength, "v%u.%u.%u", isVersionMajor, isVersionMinor, isVersionPatch);
}

void version::dump() {
    logging::snprintf("https://github.com/Strooom - %s\n", version::getIsVersionAsString());
    logging::snprintf("%s %s build - %s\n", toString(version::getBuildEnvironment()), toString(version::getBuildType()), buildInfo::buildTimeStamp);
    logging::snprintf("Creative Commons 4.0 - BY-NC-SA\n");
}