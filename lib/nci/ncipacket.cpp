#include <ncipacket.hpp>

const char* toString(messageType theMessageType) {
    switch (theMessageType) {
        case messageType::Data:
            return "Data";
            break;

        case messageType::Command:
            return "Command";
            break;

        case messageType::Response:
            return "Response";
            break;

        case messageType::Notification:
            return "Notification";
            break;

        default:
            return "other messageType";
            break;
    }
}

const char* toString(groupIdentifier theGroupIdentifier) {
    switch (theGroupIdentifier) {
        case groupIdentifier::Core:
            return "Core";
            break;

        case groupIdentifier::RfManagement:
            return "RfManagement";
            break;

        case groupIdentifier::NfceeManagement:
            return "NfceeManagement";
            break;

        case groupIdentifier::Proprietary:
            return "Proprietary";
            break;

        default:
            return "other groupIdentifier";
            break;
    }
}

const char* toString(opcodeIdentifier theOpcodeIdentifier) {
    // TODO : here I could add message type to see cmd, rsp or ntf
    switch (theOpcodeIdentifier) {
        case opcodeIdentifier::CORE_RESET_CMD:
            return "CORE_RESET";
            break;

        case opcodeIdentifier::CORE_INIT_CMD:
            return "CORE_INIT";
            break;

        default:
            return "other opcodeIdentifier";
            break;
    }
}