#pragma once

#include <stdint.h>

enum class messageType : uint8_t {
    Data         = 0x00,
    Command      = 0x20,
    Response     = 0x40,
    Notification = 0x60
};

enum class groupIdentifier : uint8_t {
    Core            = 0x00,
    RfManagement    = 0x01,
    NfceeManagement = 0x02,
    Proprietary     = 0x0F
};

enum class opcodeIdentifier : uint8_t {
    CORE_RESET_CMD = 0x00,
    CORE_RESET_RSP = 0x00,
    CORE_RESET_NTF = 0x00,

    CORE_INIT_CMD = 0x01,
    CORE_INIT_RSP = 0x01,

    CORE_SET_CONFIG_CMD = 0x02,
    CORE_SET_CONFIG_RSP = 0x02,

    CORE_GET_CONFIG_CMD = 0x03,
    CORE_GET_CONFIG_RSP = 0x03,

    CORE_CONN_CREATE_CMD = 0x04,
    CORE_CONN_CREATE_RSP = 0x04,

    CORE_CONN_CLOSE_CMD = 0x05,
    CORE_CONN_CLOSE_RSP = 0x05,

    CORE_CONN_CREDITS_NTF    = 0x06,
    CORE_GENERIC_ERROR_NTF   = 0x07,
    CORE_INTERFACE_ERROR_NTF = 0x08,

    RF_DISCOVER_CMD = 0x03,
    RF_DISCOVER_RSP = 0x03,
    RF_DISCOVER_NTF = 0x03,

    RF_DISCOVER_SELECT_CMD = 0x04,
    RF_DISCOVER_SELECT_RSP = 0x04,

    RF_INTF_ACTIVATED_NTF = 0x05,

    RF_DEACTIVATE_CMD = 0x06,
    RF_DEACTIVATE_RSP = 0x06,
    RF_DEACTIVATE_NTF = 0x06,

    // PN7160 Proprietary
    RF_TXLDO_ERROR_NTF = 0x23
};

// To simplify the code with some switch() statements, I combine the 3 message elements into a single one

enum class nciMessageId : uint16_t {
    CORE_RESET_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_RESET_CMD),
    CORE_RESET_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_RESET_RSP),
    CORE_RESET_NTF = (static_cast<uint16_t>(messageType::Notification) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_RESET_NTF),

    CORE_INIT_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_INIT_CMD),
    CORE_INIT_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_INIT_RSP),

    CORE_GET_CONFIG_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_GET_CONFIG_CMD),
    CORE_GET_CONFIG_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_GET_CONFIG_RSP),

    CORE_SET_CONFIG_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_SET_CONFIG_CMD),
    CORE_SET_CONFIG_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::Core)) << 8 | static_cast<uint16_t>(opcodeIdentifier::CORE_SET_CONFIG_RSP),

    RF_DISCOVER_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DISCOVER_CMD),
    RF_DISCOVER_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DISCOVER_RSP),
    RF_DISCOVER_NTF = (static_cast<uint16_t>(messageType::Notification) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DISCOVER_NTF),

    RF_INTF_ACTIVATED_NTF = (static_cast<uint16_t>(messageType::Notification) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_INTF_ACTIVATED_NTF),

    RF_DEACTIVATE_CMD = (static_cast<uint16_t>(messageType::Command) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DEACTIVATE_CMD),
    RF_DEACTIVATE_RSP = (static_cast<uint16_t>(messageType::Response) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DEACTIVATE_RSP),
    RF_DEACTIVATE_NTF = (static_cast<uint16_t>(messageType::Notification) | static_cast<uint16_t>(groupIdentifier::RfManagement)) << 8 | static_cast<uint16_t>(opcodeIdentifier::RF_DEACTIVATE_NTF),
};

const char* toString(nciMessageId messageId);

enum class nciStatus : uint8_t {
    // Generic Status Codes
    ok               = 0x00,
    rejected         = 0x01,
    rfFrameCorrupted = 0x02,
    failed           = 0x03,
    notInitialized   = 0x04,
    syntaxError      = 0x05,
    semanticError    = 0x06,
    // 0x07 � 0x08 RFU
    invalidParam        = 0x09,
    messageSizeExceeded = 0x0A,
    // 0x0B - 0x9F RFU

    // RF Discovery Specific Status Codes
    discoveryAlreadyStarted         = 0xA0,
    discoveryTargetActivationFailed = 0xA1,
    discoveryTearDown               = 0xA2,
    // 0xA3 - 0xAF RFU

    // RF Interface Specific Status Codes
    rfTransmissionError = 0xB0,
    rfProtocolError     = 0xB1,
    rfTimeoutError      = 0xB2,
    // 0xB3 - 0xBF RFU

    // NFCEE Interface Specific Status Codes
    nfceeInterfaceActivationFailed = 0xC0,
    nfceeTransmissionError         = 0xC1,
    nfceeProtocolError             = 0xC2,
    nfceeTimeoutError              = 0xC3
    // 0xC4 - 0xDF RFU

    // Proprietary Status Codes : 0xE0 - 0xFF
};

const char* toString(nciStatus theStatus);