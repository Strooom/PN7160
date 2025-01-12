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
#include <pn7160configcollection.hpp>

class nci {
  public:
    static void reset();
    static void run();

    static nciState getState();
    static tagStatus getTagStatus();
    static tag tagData;

#ifndef unitTesting
  private:
#endif

    static nciState state;
    static void moveState(nciState newState);

    static singleShotTimer responseTimeoutTimer;
    static singleShotTimer noTagFoundTimeoutTimer;

    static tagStatus theTagStatus;

    static constexpr uint32_t maxPayloadSize{255U};
    static constexpr uint32_t msgHeaderSize{3U};

    static uint8_t rxBuffer[maxPayloadSize + msgHeaderSize];
    static uint32_t rxMessageLength;
    static uint8_t txBuffer[maxPayloadSize + msgHeaderSize];

    static void sendNciMessage(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId, const uint8_t payloadData[] = nullptr, const uint8_t payloadLength = 0);
    static void getMessage();
    static bool checkMessageLength(const uint8_t expectedLength);
    static bool checkMessageStatus(const uint8_t receivedStatus);

    static void handleNciMessage(nciMessageId expectedMessageId, void (*handler)());

    static void handleResetResponse();
    static void handleResetNotification();
    static void handleInitResponse();
    static void handleGetConfigResponse();
    static void handleSetConfigResponse();
    static void handleRfDiscoverResponse();
    static void handleRfInterfaceActivatedNotification();
    static void handleRfDeactivationResponse();
    static void handleRfDeactivationNotification();

    static void sendGetConfig();
    static void sendSetConfig();
    static void sendStartDiscover();

    static bool configLengthMatches();
    static bool configDataMatches();

    static constexpr unsigned long scanPeriod{500};
    static constexpr unsigned long waitForReDiscoverTimeout{500};
    static void readTagData();

    static constexpr unsigned long standardResponseTimeout{10U};
    static constexpr unsigned long noTagDiscoveredTimeout{500U};
    static constexpr uint8_t resetKeepConfig{0};
    static constexpr uint8_t resetClearConfig{0x01};

    static void sendCoreReset();
    static void sendCoreInit();
    static void sendRfDeactivate();

    static void handleNoResponseTimeout();
    static void handleUnexpectedMessage();

    static messageType getMessageType(const uint8_t msgBuffer[]);
    static groupIdentifier getGroupIdentifier(const uint8_t msgBuffer[]);
    static opcodeIdentifier getOpcodeIdentifier(const uint8_t msgBuffer[]);
    static nciMessageId getMessageId(const uint8_t msgBuffer[]);
    static void dumpRawMessage(const uint8_t msgBuffer[], const uint32_t length);
};
