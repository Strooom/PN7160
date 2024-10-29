// #############################################################################
// ###                                                                       ###
// ### NFC - NCI Toolit                                                      ###
// ###                                                                       ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <ncipacket.hpp>
#include <nci.hpp>
#include <ncistatus.hpp>
#include <ncitools.hpp>
#include <logging.hpp>

messageType nciDecoder::theMessageType;
groupIdentifier nciDecoder::groupID;
opcodeIdentifier nciDecoder::opCode;
nciMessageId nciDecoder::messageId;

void nciDecoder::getMessageType(const uint8_t msgBuffer[]) {
    theMessageType = static_cast<messageType>(msgBuffer[0] & 0xE0);
}

void nciDecoder::getGroupIdentifier(const uint8_t msgBuffer[]) {
    groupID = static_cast<groupIdentifier>(msgBuffer[0] & 0x0F);
}

void nciDecoder::getOpcodeIdentifier(const uint8_t msgBuffer[]) {
    opCode = static_cast<opcodeIdentifier>(msgBuffer[1] & 0x3F);
}

bool nciDecoder::isMessageType(messageType toBeMessageType) {
    return (theMessageType == toBeMessageType);
}
bool nciDecoder::isGroupIdentifier(groupIdentifier toBeGroupID) {
    return (groupID == toBeGroupID);
}
bool nciDecoder::isOpcodeIdentifier(opcodeIdentifier toBeOpCode) {
    return (opCode == toBeOpCode);
}

bool nciDecoder::isMessage(messageType toBeMessageType, groupIdentifier toBeGroupID, opcodeIdentifier toBeOpCode) {
    return (isMessageType(toBeMessageType) && isGroupIdentifier(toBeGroupID) && isOpcodeIdentifier(toBeOpCode));
}

void nciDecoder::raw(const uint8_t msgBuffer[], const uint8_t length) {
    for (uint8_t index = 0; index < length; index++) {
        logging::snprintf("%02X ", msgBuffer[index]);
    }
    logging::snprintf("\n");
}

void nciDecoder::decode(const uint8_t msgBuffer[], const bool fullDecode) {
    uint8_t byteIndex{0};
    getMessageType(msgBuffer);
    getGroupIdentifier(msgBuffer);
    getOpcodeIdentifier(msgBuffer);
    logging::snprintf("%d..%d : %s - %s - %s[%02x]\n", byteIndex, byteIndex + 2, toString(theMessageType), toString(groupID), toString(opCode), opCode);
    byteIndex += 3;

    if (fullDecode) {
        if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_RSP)) {
            logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
            return;
        }
        if (isMessage(messageType::Notification, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_NTF)) {
            logging::snprintf("%d : reset trigger = %s\n", byteIndex, toString(static_cast<resetTrigger>(msgBuffer[byteIndex])));
            byteIndex++;
            logging::snprintf("%d : config status = TBA\n", byteIndex);
            byteIndex++;
            logging::snprintf("%d : NCI version = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : manufacturer ID = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : manufacturer specific information length = %d\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : hardwareVersion = %d\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : ROM code version = %d\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : Flash Major Version = %d\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;
            logging::snprintf("%d : Flash Minor Version = %d\n", byteIndex, msgBuffer[byteIndex]);
            byteIndex++;

            return;
        }
        if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_RSP)) {
            logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
            byteIndex++;
            logging::snprintf("%d..%d : NFCC features = 0x%02X 0x%02X 0x%02X 0x%02X\n", byteIndex, byteIndex + 3, msgBuffer[byteIndex], msgBuffer[byteIndex + 1], msgBuffer[byteIndex + 2], msgBuffer[byteIndex + 3]);
            byteIndex += 4;
            // More fields to decode if you need them = I don't
            return;
        }
        if (isMessage(messageType::Response, groupIdentifier::RfManagement, opcodeIdentifier::RF_DISCOVER_RSP)) {
            logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
            byteIndex++;
            return;
        }
    }
}
