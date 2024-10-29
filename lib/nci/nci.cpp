#include <nci.hpp>
#include <logging.hpp>
#include <pn7160interface.hpp>


nciState nci::state{nciState::boot};
tagPresentStatus nci::tagsStatus{tagPresentStatus::unknown};
uint8_t nci::rxBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
uint32_t nci::rxMessageLength;
uint8_t nci::txBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
singleTimer nci::responseTimeoutTimer;

void nci::moveState(nciState newState) {
    logging::snprintf(logging::source::stateChanges, "nci stateChange from %s (%d) to %s (%d)\n", toString(state), state, toString(newState), newState);
    state = newState;
}

void nci::reset() {
    moveState(nciState::boot);
    tagsStatus = tagPresentStatus::unknown;
}

void nci::run() {
    switch (state) {
        case nciState::boot:
            PN7160Interface::setVenPin(false);
            responseTimeoutTimer.start(3U);
            moveState(nciState::venResetActive);
            break;

        case nciState::venResetActive:
            if (responseTimeoutTimer.expired()) {
                PN7160Interface::setVenPin(true);
                responseTimeoutTimer.start(3U);
                moveState(nciState::waitForResetDone);
            }
            break;

        case nciState::waitForResetDone:
            if (responseTimeoutTimer.expired()) {
                static constexpr uint32_t payloadLength{1};
                uint8_t payloadData[payloadLength] = {resetKeepConfig};
                sendMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_CMD, payloadData, payloadLength);
                responseTimeoutTimer.start(standardResponseTimeout);
                moveState(nciState::waitForCoreResetResponse);
            }
            break;

        case nciState::waitForCoreResetResponse:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::CORE_RESET_RSP:
                        if (checkMessageStatus(rxBuffer[3])) {
                            responseTimeoutTimer.start(standardResponseTimeout);
                            moveState(nciState::waitForCoreResetNotification);
                        } else {
                            logging::snprintf(logging::source::criticalError, "Invalid CORE_RESET_RSP status\n");
                            moveState(nciState::error);
                        }
                        break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitForCoreResetNotification:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::CORE_RESET_NTF: {
                        static constexpr uint32_t payloadLength{2};
                        uint8_t payloadData[payloadLength] = {0, 0};
                        sendMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_CMD, payloadData, payloadLength);
                        responseTimeoutTimer.start(standardResponseTimeout);
                        moveState(nciState::waitforCoreInitResponse);
                    } break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitforCoreInitResponse:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::CORE_INIT_RSP:
                        configure();
                        break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitForConfigResponse:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::CORE_SET_CONFIG_RSP:
                        startDiscover();
                        break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitForDiscoverResponse:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::RF_DISCOVER_RSP:
                        moveState(nciState::waitForDiscoverNotification);
                        break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitForDiscoverNotification:
            if (PN7160Interface::hasMessage()) {
                logging::snprintf("\n\n---------------\n\n");
                getMessage();
                logging::snprintf("\n---------------\n\n");
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::RF_INTF_ACTIVATED_NTF: {
                        static constexpr uint32_t payloadLength{1};
                        uint8_t payloadData[payloadLength] = {0};        // Idle mode
                        sendMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DEACTIVATE_CMD, payloadData, payloadLength);
                        responseTimeoutTimer.start(standardResponseTimeout);
                        moveState(nciState::waitForRfDeactivationResponse);
                    } break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            }
            break;

        case nciState::waitForRfDeactivationResponse:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::RF_DEACTIVATE_RSP: {
                        responseTimeoutTimer.start(standardResponseTimeout);
                        moveState(nciState::waitForRfDeactivationNotification);
                    } break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::waitForRfDeactivationNotification:
            if (PN7160Interface::hasMessage()) {
                getMessage();
                switch (getMessageId(rxBuffer)) {
                    case nciMessageId::RF_DEACTIVATE_NTF:
                        startDiscover();
                        break;

                    default:
                        unexpectedMessageError();
                        break;
                }
            } else if (responseTimeoutTimer.expired()) {
                timeoutError();
            }
            break;

        case nciState::error:
            responseTimeoutTimer.start(1000);
            moveState(nciState::test);

            break;

        case nciState::test:
            if (responseTimeoutTimer.expired()) {
                reset();
            }
            break;

        default:
            break;
    }
}

nciState nci::getState() {
    return state;
}

tagPresentStatus nci::getTagPresentStatus() {
    return tagsStatus;
}

void nci::sendMessage(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId, const uint8_t payloadData[], const uint8_t thePayloadLength) {
    uint8_t payloadLength;
    if (payloadData == nullptr) {
        payloadLength = 0;
    } else {
        payloadLength = thePayloadLength;
    }

    txBuffer[0] = (static_cast<uint8_t>(theMessageType) | static_cast<uint8_t>(groupId)) & 0xEF;        // put messageType and groupId in first byte, Packet Boundary Flag is always 0
    txBuffer[1] = static_cast<uint8_t>(opcodeId) & 0x3F;                                                // put opcodeId in second byte, clear Reserved for Future Use (RFU) bits
    txBuffer[2] = payloadLength;                                                                        // payloadLength goes in third byte

    for (uint32_t index = 0; index < payloadLength; index++) {
        txBuffer[index + 3] = payloadData[index];
    }

    if (!PN7160Interface::wakeUp()) {
        logging::snprintf(logging::source::criticalError, "PN7160 wakeUp failed\n");
    }
    PN7160Interface::write(txBuffer, 3 + payloadLength);
}

void nci::getMessage() {
    PN7160Interface::wakeUp();
    rxMessageLength = PN7160Interface::read(rxBuffer);
    logging::snprintf("%s : ", toString(getMessageId(rxBuffer)));
    dumpRawMessage(rxBuffer, rxMessageLength);
}

bool nci::checkMessageLength(const uint8_t expectedLength) {
    return (expectedLength == rxMessageLength);
}

messageType nci::getMessageType(const uint8_t msgBuffer[]) {
    return static_cast<messageType>(msgBuffer[0] & 0xE0);
}

groupIdentifier nci::getGroupIdentifier(const uint8_t msgBuffer[]) {
    return static_cast<groupIdentifier>(msgBuffer[0] & 0x0F);
}

opcodeIdentifier nci::getOpcodeIdentifier(const uint8_t msgBuffer[]) {
    return static_cast<opcodeIdentifier>(msgBuffer[1] & 0x3F);
}

nciMessageId nci::getMessageId(const uint8_t msgBuffer[]) {
    uint16_t messageIdValue = ((static_cast<uint16_t>(msgBuffer[0] & 0xE0) | static_cast<uint16_t>(msgBuffer[0] & 0x0F)) << 8U) | static_cast<uint16_t>(msgBuffer[1] & 0x3F);
    return static_cast<nciMessageId>(messageIdValue);
}

bool nci::checkMessageStatus(const uint8_t receivedStatus) {
    return (receivedStatus == static_cast<uint8_t>(nciStatus::ok));
}

void nci::dumpRawMessage(const uint8_t msgBuffer[], const uint32_t length) {
    for (uint32_t index = 0; index < length; index++) {
        logging::snprintf("%02X ", msgBuffer[index]);
    }
    logging::snprintf("\n");
}

// void nci::decodeMessage(const uint8_t msgBuffer[], const uint8_t length, const bool fullDecode) {
//     uint8_t byteIndex{0};
//     // logging::snprintf("%d..%d : %s - %s - %s[%02x]\n", byteIndex, byteIndex + 2, toString(theMessageType), toString(groupID), toString(opCode), opCode);
//     byteIndex += 3;

//     if (fullDecode) {
//         if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_RSP)) {
//             logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
//             return;
//         }
//         if (isMessage(messageType::Notification, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_NTF)) {
//             logging::snprintf("%d : reset trigger = %s\n", byteIndex, toString(static_cast<resetTrigger>(msgBuffer[byteIndex])));
//             byteIndex++;
//             logging::snprintf("%d : config status = TBA\n", byteIndex);
//             byteIndex++;
//             logging::snprintf("%d : NCI version = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : manufacturer ID = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : manufacturer specific information length = %d\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : hardwareVersion = %d\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : ROM code version = %d\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : Flash Major Version = %d\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;
//             logging::snprintf("%d : Flash Minor Version = %d\n", byteIndex, msgBuffer[byteIndex]);
//             byteIndex++;

//             return;
//         }
//         if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_RSP)) {
//             logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
//             byteIndex++;
//             logging::snprintf("%d..%d : NFCC features = 0x%02X 0x%02X 0x%02X 0x%02X\n", byteIndex, byteIndex + 3, msgBuffer[byteIndex], msgBuffer[byteIndex + 1], msgBuffer[byteIndex + 2], msgBuffer[byteIndex + 3]);
//             byteIndex += 4;
//             // More fields to decode if you need them = I don't
//             return;
//         }
//         if (isMessage(messageType::Response, groupIdentifier::RfManagement, opcodeIdentifier::RF_DISCOVER_RSP)) {
//             logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
//             byteIndex++;
//             return;
//         }
//     }
// }

void nci::startDiscover() {
    static constexpr uint32_t payloadLength{7};
    uint8_t payloadData[payloadLength] = {3, 0, 1, 1, 1, 2, 1};
    sendMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DISCOVER_CMD, payloadData, payloadLength);
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForDiscoverResponse);
}

void nci::configure() {
    uint8_t setDurationPayload[] = {0x00, 2, 0xF4, 0x01};        // Total duration = 500ms
    sendMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_SET_CONFIG_CMD, setDurationPayload, 4);
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForConfigResponse);
}

void nci::timeoutError() {
    logging::snprintf(logging::source::criticalError, "NCI timeout error\n");
    moveState(nciState::error);
}

void nci::unexpectedMessageError() {
    logging::snprintf(logging::source::criticalError, "unexpected NCI message %s\n", toString(getMessageId(rxBuffer)));
    moveState(nciState::error);
}