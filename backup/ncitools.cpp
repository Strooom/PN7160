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
#include <ncistate.hpp>
#include <ncitools.hpp>
#include <logging.hpp>

messageType nciDecoder::getMessageType(const uint8_t msgBuffer[]) {
    return static_cast<messageType>(msgBuffer[0] & 0xE0);
}

groupIdentifier nciDecoder::getGroupIdentifier(const uint8_t msgBuffer[]) {
    return static_cast<groupIdentifier>(msgBuffer[0] & 0x0F);
}

opcodeIdentifier nciDecoder::getOpcodeIdentifier(const uint8_t msgBuffer[]) {
    return static_cast<opcodeIdentifier>(msgBuffer[1] & 0x3F);
}

nciMessageId nciDecoder::getMessageId(const uint8_t msgBuffer[]) {
    return static_cast<nciMessageId>(static_cast<uint16_t>(getMessageType(msgBuffer)) | static_cast<uint16_t>(getMessageType(msgBuffer)) << 8 | static_cast<uint16_t>(getMessageType(msgBuffer)));
}

int8_t nciDecoder::getLength(const uint8_t msgBuffer[]) {
    return msgBuffer[2];
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

void nciDecoder::decode(const uint8_t msgBuffer[], const uint8_t length, const bool fullDecode = false) {
    uint8_t byteIndex{0};
    logging::snprintf("%s ", toString(getMessageId(msgBuffer)));

    // if (fullDecode) {
    //     if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_RSP)) {
    //         logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
    //         return;
    //     }
    //     if (isMessage(messageType::Notification, groupIdentifier::Core, opcodeIdentifier::CORE_RESET_NTF)) {
    //         logging::snprintf("%d : reset trigger = %s\n", byteIndex, toString(static_cast<resetTrigger>(msgBuffer[byteIndex])));
    //         byteIndex++;
    //         logging::snprintf("%d : config status = TBA\n", byteIndex);
    //         byteIndex++;
    //         logging::snprintf("%d : NCI version = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : manufacturer ID = 0x%02X\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : manufacturer specific information length = %d\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : hardwareVersion = %d\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : ROM code version = %d\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : Flash Major Version = %d\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;
    //         logging::snprintf("%d : Flash Minor Version = %d\n", byteIndex, msgBuffer[byteIndex]);
    //         byteIndex++;

    //         return;
    //     }
    //     if (isMessage(messageType::Response, groupIdentifier::Core, opcodeIdentifier::CORE_INIT_RSP)) {
    //         logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
    //         byteIndex++;
    //         logging::snprintf("%d..%d : NFCC features = 0x%02X 0x%02X 0x%02X 0x%02X\n", byteIndex, byteIndex + 3, msgBuffer[byteIndex], msgBuffer[byteIndex + 1], msgBuffer[byteIndex + 2], msgBuffer[byteIndex + 3]);
    //         byteIndex += 4;
    //         // More fields to decode if you need them = I don't
    //         return;
    //     }
    //     if (isMessage(messageType::Response, groupIdentifier::RfManagement, opcodeIdentifier::RF_DISCOVER_RSP)) {
    //         logging::snprintf("%d : status = %s\n", byteIndex, toString(static_cast<nciStatus>(msgBuffer[byteIndex])));
    //         byteIndex++;
    //         return;
    //     }
    // }
}
