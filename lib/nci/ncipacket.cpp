#include <ncipacket.hpp>

const char* toString(nciMessageId messageId) {
    switch (messageId) {
        case nciMessageId::CORE_RESET_CMD:
            return "CORE_RESET_CMD";
        case nciMessageId::CORE_RESET_RSP:
            return "CORE_RESET_RSP";
        case nciMessageId::CORE_RESET_NTF:
            return "CORE_RESET_NTF";
        case nciMessageId::CORE_INIT_CMD:
            return "CORE_INIT_CMD";
        case nciMessageId::CORE_INIT_RSP:
            return "CORE_INIT_RSP";
        case nciMessageId::RF_DISCOVER_RSP:
            return "RF_DISCOVER_RSP";
        case nciMessageId::RF_DISCOVER_NTF:
            return "RF_DISCOVER_NTF";
        case nciMessageId::RF_INTF_ACTIVATED_NTF:
            return "RF_INTF_ACTIVATED_NTF";
        case nciMessageId::RF_DEACTIVATE_RSP:
            return "RF_DEACTIVATE_RSP";
        case nciMessageId::CORE_SET_CONFIG_RSP:
            return "CORE_SET_CONFIG_RSP";
        default:
            return "Unknow Message";
    }
}

const char* toString(nciStatus theStatus) {
    switch (theStatus) {
        case nciStatus::ok:
            return "ok";
        case nciStatus::rejected:
            return "rejected";
        case nciStatus::failed:
            return "failed";
        case nciStatus::invalidParam:
            return "invalid parameter";
        default:
            return "other statusCode";
    }
}