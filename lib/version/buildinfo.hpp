#pragma once

#include <stdint.h>
#include "buildtype.hpp"
#include "buildenvironment.hpp"
#include <ctime>
class buildInfo {
  public:
    static const buildEnvironment theBuildEnvironment;        // where was the build done : LOCAL (development PC) or CI (github actions)
    static const buildType theBuildType;                      // DEV (development build, some features such as OTA firmware update can be disabled, uart logging is enabled), PROD (release build, OTA firmware update is enabled, uart logging is disabled), INT : special build for integration testing
    static const int mainVersionDigit;
    static const int minorVersionDigit;
    static const int patchVersionDigit;
    static const char* lastCommitTag;
    static const char* buildTimeStamp;
    static const time_t buildEpoch;

    static void dump();
};
