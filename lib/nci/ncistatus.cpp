#include <ncistatus.hpp>

char* toString(nciStatus theStatus) {
    switch (theStatus) {
        case nciStatus::ok:
            return "Ok";
            break;

        case nciStatus::rejected:
            return "Rejected";
            break;

        default:
            return "other statusCode";
            break;
    }
}