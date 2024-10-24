#pragma once

#include <stdint.h>

enum class messageType : uint8_t {
    Data         = 0x00,
    Command      = 0x20,
    Response     = 0x40,
    Notification = 0x60
};

const char* toString(messageType theMessageType);

enum class groupIdentifier : uint8_t {
    Core            = 0x00,
    RfManagement    = 0x01,
    NfceeManagement = 0x02,
    Proprietary     = 0x0F
};

const char* toString(groupIdentifier theGroupIdentifier);

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

    CORE_CONN_CREDITS_NTF = 0x06,
    CORE_GENERIC_ERROR_NTF = 0x07,
    CORE_INTERFACE_ERROR_NTF = 0x08,

    RF_DISCOVER_MAP_CMD = 0x00,
    RF_DISCOVER_MAP_RSP = 0x00,

    RF_SET_LISTEN_MODE_ROUTING_CMD = 0x01
};

const char* toString(opcodeIdentifier theOpcodeIdentifier);


// #define RF_SET_LISTEN_MODE_ROUTING_CMD 0x01
// #define RF_SET_LISTEN_MODE_ROUTING_RSP 0x01

// #define RF_GET_LISTEN_MODE_ROUTING_CMD 0x02
// #define RF_GET_LISTEN_MODE_ROUTING_RSP 0x02
// #define RF_GET_LISTEN_MODE_ROUTING_NTF 0x02

// #define RF_DISCOVER_CMD 0x03
// #define RF_DISCOVER_RSP 0x03
// #define RF_DISCOVER_NTF 0x03

// #define RF_DISCOVER_SELECT_CMD 0x04
// #define RF_DISCOVER_SELECT_RSP 0x04

// #define RF_INTF_ACTIVATED_NTF 0x05

// #define RF_DEACTIVATE_CMD 0x06
// #define RF_DEACTIVATE_RSP 0x06
// #define RF_DEACTIVATE_NTF 0x06

// #define RF_FIELD_INFO_NTF 0x07

// #define RF_T3T_POLLING_CMD 0x08
// #define RF_T3T_POLLING_RSP 0x08
// #define RF_T3T_POLLING_NTF 0x08
// #define RF_NFCEE_ACTION_NTF 0x09
// #define RF_NFCEE_DISCOVERY_REQ_NTF 0x0A
// #define RF_PARAMETER_UPDATE_CMD 0x0B
// #define RF_PARAMETER_UPDATE_RSP 0x0B
// // 1100b -  1111b RFU

// #define NFCEE_DISCOVER_CMD 0x00
// #define NFCEE_DISCOVER_RSP 0x00
// #define NFCEE_DISCOVER_NTF 0x00
// #define NFCEE_MODE_SET_CMD 0x01
// #define NFCEE_MODE_SET_RSP 0x01
// // 0010b - 1111b RFU

// #define NCI_PROPRIETARY_ACT_CMD 0x02        // See PN7150 Datasheet, section 5.4
// #define NCI_PROPRIETARY_ACT_RSP 0x02        // See PN7150 Datasheet, section 5.4, Table 23 and 24

