#include "buildenvironment.hpp"

const char* toString(buildEnvironment aBuildEnvironment) {
    switch (aBuildEnvironment) {
        case buildEnvironment::local:
            return "local";
            break;

        case buildEnvironment::ci:
            return "ci";
            break;

        default:
            return "unknown";
            break;
    }
}
