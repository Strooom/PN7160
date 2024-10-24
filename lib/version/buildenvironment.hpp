#pragma once
#include <stdint.h>

enum class buildEnvironment {
    unknown,
    local,
    ci,
};

const char* toString(buildEnvironment aBuildEnvironment);