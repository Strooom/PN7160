#include "buildtype.hpp"

const char* toString(buildType aBuildType) {
    switch (aBuildType) {
        case buildType::development:
            return "development";
            break;

        case buildType::production:
            return "production";
            break;

        case buildType::integrationTest:
            return "integrationTest";
            break;

        default:
            return "unknown";
            break;
    }
}

