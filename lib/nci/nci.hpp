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

enum class tagPresentStatus : uint8_t {
    unknown,
    noTagPresent,
    newTagPresent,
    oldTagPresent
};

class nci {
  public:
    static void startDiscover();
    // static void deActivate(NciRfDeAcivationMode theMode);
    static void run();
    static void reset();

    static nciState getState();
    static tagPresentStatus getTagPresentStatus();

    static bool checkMessageType(const messageType theMessageType, const groupIdentifier groupId, const opcodeIdentifier opcodeId);
    static messageType getMessageType(const uint8_t msgBuffer[]);
    static groupIdentifier getGroupIdentifier(const uint8_t msgBuffer[]);
    static opcodeIdentifier getOpcodeIdentifier(const uint8_t msgBuffer[]);
    static nciMessageId getMessageId(const uint8_t msgBuffer[]);

    static void dumpRawMessage(const uint8_t msgBuffer[], const uint8_t length) ;
    // static void decodeMessage(const uint8_t msgBuffer[], const uint8_t length, const bool fullDecode = false) ;


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

};

#define ResetKeepConfig 0x00
#define ResetClearConfig 0x01

// ---------------------------------------------------
// NCI Status Codes. NCI Specification V1.0 - Table 94
// ---------------------------------------------------

// Generic Status Codes
#define STATUS_OK 0x00
#define STATUS_REJECTED 0x01
#define STATUS_RF_FRAME_CORRUPTED 0x02
#define STATUS_FAILED 0x03
#define STATUS_NOT_INITIALIZED 0x04
#define STATUS_SYNTAX_ERROR 0x05
#define STATUS_SEMANTIC_ERROR 0x06
// 0x07 � 0x08 RFU
#define STATUS_INVALID_PARAM 0x09
#define STATUS_MESSAGE_SIZE_EXCEEDED 0x0A
// 0x0B - 0x9F RFU

// RF Discovery Specific Status Codes
#define DISCOVERY_ALREADY_STARTED 0xA0
#define DISCOVERY_TARGET_ACTIVATION_FAILED 0xA1
#define DISCOVERY_TEAR_DOWN 0xA2
// 0xA3 - 0xAF RFU

// RF Interface Specific Status Codes
#define RF_TRANSMISSION_ERROR 0xB0
#define RF_PROTOCOL_ERROR 0xB1
#define RF_TIMEOUT_ERROR 0xB2
// 0xB3 - 0xBF RFU

// NFCEE Interface Specific Status Codes
#define NFCEE_INTERFACE_ACTIVATION_FAILED 0xC0
#define NFCEE_TRANSMISSION_ERROR 0xC1
#define NFCEE_PROTOCOL_ERROR 0xC2
#define NFCEE_TIMEOUT_ERROR 0xC3
// 0xC4 - 0xDF RFU

// Proprietary Status Codes : 0xE0 - 0xFF

// ------------------------------------------------------
// NCI RF Technologies. NCI Specification V1.0 - Table 95
// ------------------------------------------------------

#define NFC_RF_TECHNOLOGY_A 0x00
#define NFC_RF_TECHNOLOGY_B 0x01
#define NFC_RF_TECHNOLOGY_F 0x02
#define NFC_RF_TECHNOLOGY_15693 0x03
// 0x04 � 0x7F RFU
// 0x80 - 0xFE For proprietary use
// 0xFF RFU

// -------------------------------------------------------------
// NCI RF Technology and Mode. NCI Specification V1.0 - Table 96
// -------------------------------------------------------------

#define NFC_A_PASSIVE_POLL_MODE 0x00
#define NFC_B_PASSIVE_POLL_MODE 0x01
#define NFC_F_PASSIVE_POLL_MODE 0x02
#define NFC_A_ACTIVE_POLL_MODE 0x03
// RFU	0x04
#define NFC_F_ACTIVE_POLL_MODE 0x05
#define NFC_15693_PASSIVE_POLL_MODE 0x06
// 0x07 � 0x6F RFU
// 0x70 � 0x7F Reserved for Proprietary Technologies in Poll Mode
#define NFC_A_PASSIVE_LISTEN_MODE 0x80
#define NFC_B_PASSIVE_LISTEN_MODE 0x81
#define NFC_F_PASSIVE_LISTEN_MODE 0x82
#define NFC_A_ACTIVE_LISTEN_MODE 0x83
// RFU	0x84
#define NFC_F_ACTIVE_LISTEN_MODE 0x85
#define NFC_15693_PASSIVE_LISTEN_MODE 0x86
// 0x87 � 0xEF RFU
// 0xF0 � 0xFF Reserved for Proprietary Technologies in Listen Mode

// ---------------------------------------------------
// NCI RF Protocols. NCI Specification V1.0 - Table 98
// ---------------------------------------------------

#define PROTOCOL_UNDETERMINED 0x00
#define PROTOCOL_T1T 0x01
#define PROTOCOL_T2T 0x02
#define PROTOCOL_T3T 0x03
#define PROTOCOL_ISO_DEP 0x04
#define PROTOCOL_NFC_DEP 0x05
// 0x06 � 0x7F RFU
// 0x80-0xFE For proprietary use
// 0xFF RFU

// -----------------------------------------------------
// NCI RF Interfacess. NCI Specification V1.0 - Table 99
// -----------------------------------------------------

#define NFCEE_Direct_RF_Interface 0x00
#define Frame_RF_interface 0x01
#define ISO_DEP_RF_interface 0x02
#define NFC_DEP_RF_interface 0x03
// 0x04 � 0x7F RFU
// 0x80 - 0xFE For proprietary use
// 0xFF RFU

// ---------------------------------------------------------------
// NFCEE Protocol / Interfaces. NCI Specification V1.0 - Table 100
// ---------------------------------------------------------------

#define APDU 0x00
#define HCI Access 0x01
#define Type 3 Tag Command Set 0x02
#define Transparent 0x03
// 0x04 - 0x7F RFU
// 0x80 - 0xFE For proprietary use
// 0xFF RFU

// --------------------------------------------
// Bit Rates. NCI Specification V1.0 - Table 97
// --------------------------------------------

#define NFC_BIT_RATE_106 0x00
#define NFC_BIT_RATE_212 0x01
#define NFC_BIT_RATE_424 0x02
#define NFC_BIT_RATE_848 0x03
#define NFC_BIT_RATE_1695 0x04
#define NFC_BIT_RATE_3390 0x05
#define NFC_BIT_RATE_6780 0x06
// 0x07 � 0x7F RFU
// 0x80 - 0xFE For proprietary use
// 0xFF RFU

// ----------------------------------------------------------------------
// Dectivation Types for RF_DEACTIVATE_ NCI Specification V1.0 - Table 63
// ----------------------------------------------------------------------

enum class NciRfDeAcivationMode : uint8_t {
    IdleMode     = 0x00,
    SleepMode    = 0x01,
    Sleep_AFMode = 0x02,
    Discovery    = 0x03
    // 0x04 � 0xFF RFU
};

// ----------------------------------------------------------------------
// Dectivation Reason RF_DEACTIVATE_NTF NCI Specification V1.0 - Table 64
// ----------------------------------------------------------------------

#define DH_Request 0x00
#define Endpoint_Request 0x01
#define RF_Link_Loss 0x02
#define NFC_B_Bad_AFI 0x03
// 0x04 � 0xFF RFU

/* Discovery Types/Detected Technology and Mode */
#define NCI_DISCOVERY_TYPE_POLL_A 0x00
#define NCI_DISCOVERY_TYPE_POLL_B 0x01
#define NCI_DISCOVERY_TYPE_POLL_F 0x02
#define NCI_DISCOVERY_TYPE_POLL_A_ACTIVE 0x03
#define NCI_DISCOVERY_TYPE_POLL_F_ACTIVE 0x05
#define NCI_DISCOVERY_TYPE_POLL_ISO15693 0x06

enum class notificationType : uint8_t {
    lastNotification          = 0x00,
    lastNotificationNfccLimit = 0x01,
    moreNotification          = 0x02
    // 	3 - 255 RFU
};

// ------------------------------------------------------------------------------------------
// NFC Requires the device to behave according to a certain State Machine.
// Furthermore the NCI protocol consists of Commands, to be followed by Responses
// All this behaviour is implemented with a Finite State Machine, with following states
// ------------------------------------------------------------------------------------------

enum class nciError : uint8_t {
    responseNOK,            // we received a response with somethin wrong in it, eg Status_NOK
    responseTimeout,        // we did not receive a response in time
    none
};
