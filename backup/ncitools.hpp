// #############################################################################
// ###                                                                       ###
// ### NFC - NCI Toolit                                                      ###
// ###                                                                       ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#pragma once

#include <logging.hpp>
#include <nci.hpp>
#include <ncistate.hpp>

class nciDecoder {
  public:
    static void raw(const uint8_t msgBuffer[], const uint8_t length);
    static void decode(const uint8_t msgBuffer[], const uint8_t length, const bool fullDecode = false);

    static bool isMessageType(messageType toBeMessageType);
    static bool isGroupIdentifier(groupIdentifier toBeGroupID);
    static bool isOpcodeIdentifier(opcodeIdentifier toBeOpCode);
    static bool isMessage(messageType toBeMessageType, groupIdentifier toBeGroupID, opcodeIdentifier toBeOpCode);

    static messageType theMessageType;
    static groupIdentifier groupID;
    static opcodeIdentifier opCode;
    static nciMessageId messageId;

    static messageType getMessageType(const uint8_t msgBuffer[]);
    static groupIdentifier getGroupIdentifier(const uint8_t msgBuffer[]);
    static opcodeIdentifier getOpcodeIdentifier(const uint8_t msgBuffer[]);
    static nciMessageId getMessageId(const uint8_t msgBuffer[]);
    static int8_t getLength(const uint8_t msgBuffer[]);
};
