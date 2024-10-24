#pragma once
#include <stdint.h>

enum class buildType {
    unknown,
    development,
    production,
    integrationTest,
};

const char* toString(buildType aBuildType);