#pragma once

// #############################################################################
// ###                                                                       ###
// ### NXP PN7160 Driver                                                     ###
// ###                                                                       ###
// ### https://github.com/Strooom/PN7160                                     ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <stdint.h>
#include <tag.hpp>
#include <pn7160interface.hpp>
#include <ncistate.hpp>
#include <ncipacket.hpp>
#include <singletimer.hpp>


class nci {
  public:
    static void startDiscover();

    static void run();
    static void reset();

    static nciState getState();
    static tagPresentStatus getTagPresentStatus();

    static bool checkMessageType(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId);
    static messageType getMessageType(const uint8_t msgBuffer[]);
    static groupIdentifier getGroupIdentifier(const uint8_t msgBuffer[]);
    static opcodeIdentifier getOpcodeIdentifier(const uint8_t msgBuffer[]);
    static nciMessageId getMessageId(const uint8_t msgBuffer[]);

    static void dumpRawMessage(const uint8_t msgBuffer[], const uint32_t length);

#ifndef unitTesting
  private:
#endif

    static nciState state;
    static void moveState(nciState newState);

    static singleTimer responseTimeoutTimer;

    static tagPresentStatus tagsStatus;
    static tag theTag;

    static constexpr uint32_t maxPayloadSize{255U};
    static constexpr uint32_t msgHeaderSize{3U};

    static uint8_t rxBuffer[maxPayloadSize + msgHeaderSize];        // buffer where we store bytes received until they form a complete message
    static uint32_t rxMessageLength;                                // length of the last message received. As these are not 0x00 terminated, we need to remember the length
    static uint8_t txBuffer[maxPayloadSize + msgHeaderSize];        // buffer where we store the message to be transmitted

    static void sendMessage(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId, const uint8_t payloadData[] = nullptr, const uint8_t payloadLength = 0);
    static void getMessage();
    static bool checkMessageLength(const uint8_t expectedLength);
    static bool checkMessageStatus(const uint8_t receivedStatus);

    static void configure();
    static void timeoutError();
    static void unexpectedMessageError();

    static constexpr unsigned long scanPeriod{500};
    static constexpr unsigned long waitForReDiscoverTimeout{500};
    void saveTag(uint8_t msgType);

    static constexpr unsigned long standardResponseTimeout{10U};
    static constexpr uint8_t resetKeepConfig{0};
    static constexpr uint8_t resetClearConfig{0x01};
};

