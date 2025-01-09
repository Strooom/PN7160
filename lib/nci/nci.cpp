#include <nci.hpp>
#include <logging.hpp>
#include <pn7160interface.hpp>
#include <pn7160configcollection.hpp>
#include <cstring>

nciState nci::state{nciState::boot};
uint8_t nci::rxBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
uint32_t nci::rxMessageLength;
uint8_t nci::txBuffer[nci::maxPayloadSize + nci::msgHeaderSize];
singleShotTimer nci::responseTimeoutTimer;
singleShotTimer nci::noTagFoundTimoutTimer;
pn7160configcollection nci::pn7160configuration;
tagStatus nci::theTagStatus{tagStatus::absent};
tag nci::tagData;

void nci::run() {
    switch (state) {
        case nciState::boot:
            pn7160configcollection::activeConfig = 0;
            PN7160Interface::setVenPin(false);
            responseTimeoutTimer.start(3U);
            moveState(nciState::venResetActive);
            break;

        case nciState::venResetActive:
            if (responseTimeoutTimer.isExpired()) {
                PN7160Interface::setVenPin(true);
                responseTimeoutTimer.start(3U);
                moveState(nciState::waitForHwReset);
            }
            break;

        case nciState::waitForHwReset:
            if (responseTimeoutTimer.isExpired()) {
                sendCoreReset();
                responseTimeoutTimer.start(standardResponseTimeout);
                moveState(nciState::waitForCoreResetResponse);
            }
            break;

        case nciState::waitForCoreResetResponse:
            handleNciMessage(nciMessageId::CORE_RESET_RSP, handleResetResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForCoreResetNotification:
            handleNciMessage(nciMessageId::CORE_RESET_NTF, handleResetNotification);
            handleNoResponseTimeout();
            break;

        case nciState::waitForCoreInitResponse:
            handleNciMessage(nciMessageId::CORE_INIT_RSP, handleInitResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForGetConfigResponse:
            handleNciMessage(nciMessageId::CORE_GET_CONFIG_RSP, handleGetConfigResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForSetConfigResponse:
            handleNciMessage(nciMessageId::CORE_SET_CONFIG_RSP, handleSetConfigResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForDiscoverResponse:
            handleNciMessage(nciMessageId::RF_DISCOVER_RSP, handleRfDiscoverResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForRfInterfaceActivatedNotification:
            handleNciMessage(nciMessageId::RF_INTF_ACTIVATED_NTF, handleRfInterfaceActivatedNotification);
            if (noTagFoundTimoutTimer.isExpired()) {
                switch (theTagStatus) {
                    case tagStatus::foundNew:
                    case tagStatus::old:
                        theTagStatus = tagStatus::removed;
                        tagData.setUniqueId(0, nullptr);
                        logging::snprintf(logging::source::tagEvents, "Tag data cleared\n");
                        break;
                    default:
                        break;
                }
            }
            break;

        case nciState::waitForRfDeactivateResponse:
            handleNciMessage(nciMessageId::RF_DEACTIVATE_RSP, handleRfDeactivationResponse);
            handleNoResponseTimeout();
            break;

        case nciState::waitForRfDeactivateNotification:
            handleNciMessage(nciMessageId::RF_DEACTIVATE_NTF, handleRfDeactivationNotification);
            handleNoResponseTimeout();
            break;

        case nciState::waitForRestartDiscovery:
            if (responseTimeoutTimer.isExpired()) {
                sendStartDiscover();
                responseTimeoutTimer.start(standardResponseTimeout);
                noTagFoundTimoutTimer.start(noTagDiscoverdTimeout);
                moveState(nciState::waitForDiscoverResponse);
            }
            break;

        case nciState::error:
            break;

        default:
            break;
    }
}

void nci::sendCoreReset() {
    static constexpr uint32_t payloadLength{1};
    uint8_t payloadData[payloadLength] = {resetKeepConfig};
    sendNciMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_CMD, payloadData, payloadLength);
}

void nci::handleResetResponse() {
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForCoreResetNotification);
}

void nci::handleResetNotification() {
    sendCoreInit();
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForCoreInitResponse);
}

void nci::sendCoreInit() {
    static constexpr uint32_t payloadLength{2};
    uint8_t payloadData[payloadLength] = {0, 0};
    sendNciMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_CMD, payloadData, payloadLength);
}

void nci::handleInitResponse() {
    if (pn7160configcollection::activeConfig >= pn7160configcollection::nmbrOfConfigs) {
        sendStartDiscover();
        responseTimeoutTimer.start(standardResponseTimeout);
        noTagFoundTimoutTimer.start(noTagDiscoverdTimeout);
        moveState(nciState::waitForDiscoverResponse);
    } else {
        sendGetConfig();
        responseTimeoutTimer.start(standardResponseTimeout);
        moveState(nciState::waitForGetConfigResponse);
    }
}

void nci::sendGetConfig() {
    uint8_t payload[3]{};        // CORE_GET_CONFIG_CMD has 2 bytes payload for normal tags, and 3 bytes for PN7160 extended tags
    payload[0] = 1;              // to keep it simple, we query all configs one by one
    uint8_t payloadLength;
    if (pn7160configuration.configs[pn7160configcollection::activeConfig].tag <= 0xFF) {
        payload[1]    = pn7160configuration.configs[pn7160configcollection::activeConfig].tag;
        payloadLength = 2;
    } else {
        payload[1]    = pn7160configuration.configs[pn7160configcollection::activeConfig].tag >> 8;
        payload[2]    = pn7160configuration.configs[pn7160configcollection::activeConfig].tag & 0xFF;
        payloadLength = 3;
    }
    sendNciMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_GET_CONFIG_CMD, payload, payloadLength);
}

void nci::handleGetConfigResponse() {
    if (configLengthMatches() && configDataMatches()) {
        pn7160configcollection::activeConfig++;
        if (pn7160configcollection::activeConfig >= pn7160configcollection::nmbrOfConfigs) {
            sendStartDiscover();
            responseTimeoutTimer.start(standardResponseTimeout);
            noTagFoundTimoutTimer.start(noTagDiscoverdTimeout);

            moveState(nciState::waitForDiscoverResponse);
        } else {
            sendGetConfig();
            responseTimeoutTimer.start(standardResponseTimeout);
            moveState(nciState::waitForGetConfigResponse);
        }
    } else {
        sendSetConfig();
        responseTimeoutTimer.start(standardResponseTimeout);
        moveState(nciState::waitForSetConfigResponse);
    }
}

void nci::handleSetConfigResponse() {
    pn7160configcollection::activeConfig++;
    if (pn7160configcollection::activeConfig >= pn7160configcollection::nmbrOfConfigs) {
        sendStartDiscover();
        responseTimeoutTimer.start(standardResponseTimeout);
        noTagFoundTimoutTimer.start(noTagDiscoverdTimeout);
        moveState(nciState::waitForDiscoverResponse);
    } else {
        sendGetConfig();
        responseTimeoutTimer.start(standardResponseTimeout);
        moveState(nciState::waitForGetConfigResponse);
    }
}

void nci::handleRfDiscoverResponse() {
    responseTimeoutTimer.stop();
    moveState(nciState::waitForRfInterfaceActivatedNotification);
}

void nci::handleRfInterfaceActivatedNotification() {
    logging::snprintf("tag detected...\n");
    updateTag();
    sendRfDeactivate();
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForRfDeactivateResponse);
}

void nci::handleRfDeactivationResponse() {
    responseTimeoutTimer.start(standardResponseTimeout);
    moveState(nciState::waitForRfDeactivateNotification);
}

void nci::handleRfDeactivationNotification() {
    responseTimeoutTimer.start(500U);
    moveState(nciState::waitForRestartDiscovery);
}

void nci::unexpectedMessageError() {
    logging::snprintf(logging::source::criticalError, "unexpected NCI message %s : ", toString(getMessageId(rxBuffer)));
    dumpRawMessage(rxBuffer, rxMessageLength);
    moveState(nciState::error);
}

void nci::sendStartDiscover() {
    static constexpr uint32_t payloadLength{7};
    uint8_t payloadData[payloadLength] = {3, 0, 1, 1, 1, 2, 1};
    sendNciMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DISCOVER_CMD, payloadData, payloadLength);
}


nciState nci::getState() {
    return state;
}

tagStatus nci::getTagStatus() {
    tagStatus result{theTagStatus};
    switch (theTagStatus) {
        case tagStatus::foundNew:
            theTagStatus = tagStatus::old;
            break;
        case tagStatus::removed:
            theTagStatus = tagStatus::absent;
            break;

        default:
            break;
    }
    return result;
}

void nci::sendNciMessage(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId, const uint8_t payloadData[], const uint8_t thePayloadLength) {
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
        logging::snprintf(logging::source::criticalError, "PN7160 wakeUp for sendNciMessage failed\n");
    }
    PN7160Interface::write(txBuffer, 3 + payloadLength);
    if (logging::isActive(logging::source::nciMessages)) {
        logging::snprintf("%s : ", toString(getMessageId(txBuffer)));
        dumpRawMessage(txBuffer, 3 + payloadLength);
    }
}

void nci::getMessage() {
    if (!PN7160Interface::wakeUp()) {
        logging::snprintf(logging::source::criticalError, "PN7160 wakeUp for receiveMessage failed\n");
    }
    rxMessageLength = PN7160Interface::read(rxBuffer);
    if (logging::isActive(logging::source::nciMessages)) {
        logging::snprintf("%s : ", toString(getMessageId(rxBuffer)));
        dumpRawMessage(rxBuffer, rxMessageLength);
    }
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

bool nci::configLengthMatches() {
    uint8_t expectedLength{0};
    if (pn7160configuration.configs[pn7160configcollection::activeConfig].tag > 0x00FF) {
        expectedLength = 3 + 1 + 1 + 2 + 1 + pn7160configuration.configs[pn7160configcollection::activeConfig].length;
    } else {
        expectedLength = 3 + 1 + 1 + 1 + 1 + pn7160configuration.configs[pn7160configcollection::activeConfig].length;
    }
    bool lengthMatches = (rxMessageLength == expectedLength);
    if ((!lengthMatches) && (logging::isActive(logging::source::nciMessages))) {
        logging::snprintf(logging::source::nciMessages, "configLength ToBe %d - AsIs %d\n", expectedLength, rxMessageLength);
    }
    return lengthMatches;
}

bool nci::configDataMatches() {
    uint8_t configOffset{0};
    if (pn7160configuration.configs[pn7160configcollection::activeConfig].tag > 0x00FF) {
        configOffset = 3 + 1 + 1 + 2 + 1;
    } else {
        configOffset = 3 + 1 + 1 + 1 + 1;
    }

    bool configDataMatch = (memcmp(rxBuffer + configOffset, pn7160configuration.configs[pn7160configcollection::activeConfig].data, pn7160configuration.configs[pn7160configcollection::activeConfig].length) == 0);
    if ((!configDataMatch) && (logging::isActive(logging::source::nciMessages))) {
        logging::snprintf("configData mismatch : ToBe : ");
        for (uint32_t index = 0; index < pn7160configuration.configs[pn7160configcollection::activeConfig].length; index++) {
            logging::snprintf("%02X ", pn7160configuration.configs[pn7160configcollection::activeConfig].data[index]);
        }
        logging::snprintf("--- AsIs : ");
        for (uint32_t index = 0; index < pn7160configuration.configs[pn7160configcollection::activeConfig].length; index++) {
            logging::snprintf("%02X ", rxBuffer[configOffset + index]);
        }
        logging::snprintf("\n");
    }

    return configDataMatch;
}

void nci::sendSetConfig() {
    uint8_t configPayload[pn7160config::maxLenght + 3]{};
    uint8_t configDataOffset{0};
    uint8_t payloadLength{0};

    configPayload[0] = 1;        // 1 configure 1 parameter at a time
    payloadLength++;
    configDataOffset++;

    if (pn7160configuration.configs[pn7160configcollection::activeConfig].tag <= 0xFF) {
        configPayload[1] = pn7160configuration.configs[pn7160configcollection::activeConfig].tag;
        payloadLength += 1;
        configDataOffset += 1;
    } else {
        configPayload[1] = pn7160configuration.configs[pn7160configcollection::activeConfig].tag >> 8;
        configPayload[2] = pn7160configuration.configs[pn7160configcollection::activeConfig].tag & 0xFF;
        payloadLength += 2;
        configDataOffset += 2;
    }

    configPayload[configDataOffset] = pn7160configuration.configs[pn7160configuration.activeConfig].length;
    payloadLength++;
    configDataOffset++;

    memcpy(configPayload + configDataOffset, pn7160configuration.configs[pn7160configcollection::activeConfig].data, pn7160configuration.configs[pn7160configuration.activeConfig].length);
    payloadLength += pn7160configuration.configs[pn7160configcollection::activeConfig].length;

    sendNciMessage(messageType::Command, groupIdentifier::Core, opcodeIdentifier::CORE_SET_CONFIG_CMD, configPayload, payloadLength);
}

void nci::updateTag() {
    tag receivedTag;
    static constexpr uint8_t byeOffset{12};
    receivedTag.setUniqueId(rxBuffer[byeOffset], rxBuffer + byeOffset + 1);
    if (receivedTag == tagData) {
        theTagStatus = tagStatus::old;
    } else {
        theTagStatus = tagStatus::foundNew;
        tagData.setUniqueId(rxBuffer[byeOffset], rxBuffer + byeOffset + 1);
    }
}

void nci::handleNciMessage(nciMessageId expectedMessageId, void (*handler)()) {
    if (PN7160Interface::hasMessage()) {
        getMessage();
        if (getMessageId(rxBuffer) == expectedMessageId) {
            handler();
        } else {
            unexpectedMessageError();
            moveState(nciState::error);
        }
    }
}

void nci::handleNoResponseTimeout() {
    if (responseTimeoutTimer.isExpired()) {
        logging::snprintf(logging::source::criticalError, "NCI response timeout error\n");
        moveState(nciState::error);
    }
}

void nci::reset() {
    moveState(nciState::boot);
    theTagStatus = tagStatus::absent;
}

void nci::sendRfDeactivate() {
    static constexpr uint32_t payloadLength{1};
    uint8_t payloadData[payloadLength] = {0};        // Idle mode
    sendNciMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DEACTIVATE_CMD, payloadData, payloadLength);
}

void nci::moveState(nciState newState) {
    logging::snprintf(logging::source::stateChanges, "nci stateChange from %s (%d) to %s (%d)\n", toString(state), state, toString(newState), newState);
    state = newState;
}
