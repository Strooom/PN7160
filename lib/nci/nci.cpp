#include <nci.hpp>
#include <logging.hpp>
#include <pn7160interface.hpp>

nciState nci::state{nciState::boot};
tagStatus nci::theTagStatus{tagStatus::noTag};
tag nci::tagData;

uint8_t nci::rxBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
uint32_t nci::rxMessageLength;
uint8_t nci::txBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
singleShotTimer nci::responseTimeoutTimer;

void nci::moveState(nciState newState) {
    logging::snprintf(logging::source::stateChanges, "nci stateChange from %s (%d) to %s (%d)\n", toString(state), state, toString(newState), newState);
    state = newState;
}

void nci::reset() {
    moveState(nciState::boot);
    theTagStatus = tagStatus::noTag;
}

void nci::run() {
    switch (state) {
        case nciState::boot:
            PN7160Interface::setVenPin(false);
            responseTimeoutTimer.start(3U);
            moveState(nciState::venResetActive);
            break;

        case nciState::venResetActive:
            if (responseTimeoutTimer.isExpired()) {
                PN7160Interface::setVenPin(true);
                responseTimeoutTimer.start(3U);
                moveState(nciState::waitForResetDone);
            }
            break;

        case nciState::waitForResetDone:
            if (responseTimeoutTimer.isExpired()) {
                sendCoreReset();
            }
            break;

        case nciState::waitForCoreResetResponse:
            waitForMessage(nciMessageId::CORE_RESET_RSP, nciState::waitForCoreResetNotification, standardResponseTimeout);
            break;

        case nciState::waitForCoreResetNotification:
            waitForMessage(nciMessageId::CORE_RESET_NTF, nciState::waitforCoreInitResponse, sendCoreInit);
            break;

        case nciState::waitforCoreInitResponse:
            waitForMessage(nciMessageId::CORE_INIT_RSP, nciState::waitforCoreInitResponse, configure);
            break;

        case nciState::waitForConfigResponse:
            waitForMessage(nciMessageId::CORE_SET_CONFIG_RSP, nciState::waitforCoreInitResponse, startDiscover);
            break;

        case nciState::waitForDiscoverResponse:
            waitForMessage(nciMessageId::RF_DISCOVER_RSP, nciState::waitForDiscoverNotification, noTagDiscoverdTimeout);
            break;

        case nciState::waitForDiscoverNotification:
            waitForMessage(nciMessageId::RF_INTF_ACTIVATED_NTF, nciState::waitForRfDeactivationResponse, sendDeactivate);
            break;

        case nciState::waitForRfDeactivationResponse:
            waitForMessage(nciMessageId::RF_DEACTIVATE_RSP, nciState::waitForRfDeactivationNotification, standardResponseTimeout);
            break;

        case nciState::waitForRfDeactivationNotification:
            waitForMessage(nciMessageId::RF_DEACTIVATE_NTF, nciState::waitForRestartDiscovery, 500U);
            break;

        case nciState::waitForRestartDiscovery:
            if (responseTimeoutTimer.isExpired()) {
                startDiscover();
            }
            break;

        case nciState::error:
            // For the time being, we end here... Later it should reset a number of trials to repair occasional errors...
            break;

        default:
            break;
    }
}

nciState nci::getState() {
    return state;
}

tagStatus nci::getTagPresentStatus() {
    return theTagStatus;
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
    uint16_t messageType    = static_cast<uint16_t>(msgBuffer[0]) & 0x00E0;
    uint16_t groupId        = static_cast<uint16_t>(msgBuffer[0]) & 0x000F;
    uint16_t firstBytes     = static_cast<uint16_t>((messageType | groupId) << 8U);
    uint16_t opcodeId       = static_cast<uint16_t>(msgBuffer[1]) & 0x003F;
    uint16_t messageIdValue = static_cast<uint16_t>(firstBytes | opcodeId);
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

void nci::updateTag() {
    tag receivedTag;
    static constexpr uint8_t byeOffset{12};        // TODO : not sure this offset is valid for all types of tags..
    receivedTag.setUniqueId(rxBuffer[byeOffset], rxBuffer + byeOffset + 1);

    if (receivedTag == tagData) {
        theTagStatus = tagStatus::oldTag;
    } else {
        theTagStatus = tagStatus::newTag;
        tagData      = receivedTag;
    }
}

void nci::waitForMessage(nciMessageId expectedMessageId, nciState nextState, void (*doAction)()) {
    if (PN7160Interface::hasMessage()) {
        getMessage();
        if (expectedMessageId == getMessageId(rxBuffer)) {
            doAction();
            moveState(nextState);
        } else {
            unexpectedMessageError();
            moveState(nciState::error);
        }
    } else if (responseTimeoutTimer.isExpired()) {
        timeoutError();
        moveState(nciState::error);
    }
}

void nci::waitForMessage(nciMessageId expectedMessageId, nciState nextState, unsigned long timeout) {
    if (PN7160Interface::hasMessage()) {
        getMessage();
        if (expectedMessageId == getMessageId(rxBuffer)) {
            responseTimeoutTimer.start(timeout);
            moveState(nextState);
        } else {
            unexpectedMessageError();
            moveState(nciState::error);
        }
    } else if (responseTimeoutTimer.isExpired()) {
        timeoutError();
        moveState(nciState::error);
    }
}

void nci::sendCoreReset() {
    static constexpr uint32_t payloadLength{1};
    uint8_t payloadData[payloadLength] = {resetKeepConfig};
    sendMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_CMD, payloadData, payloadLength);
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForCoreResetResponse);
}

void nci::sendCoreInit() {
    static constexpr uint32_t payloadLength{2};
    uint8_t payloadData[payloadLength] = {0, 0};
    sendMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_CMD, payloadData, payloadLength);
    responseTimeoutTimer.start(standardResponseTimeout);
}

void nci::sendDeactivate() {
    static constexpr uint32_t payloadLength{1};
    uint8_t payloadData[payloadLength] = {0};        // Idle mode
    sendMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DEACTIVATE_CMD, payloadData, payloadLength);
    responseTimeoutTimer.start(standardResponseTimeout);
}