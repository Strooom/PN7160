#pragma once

// ---------------------------------------------------
// NCI Status Codes. NCI Specification V1.0 - Table 94
// ---------------------------------------------------

enum class nciStatus {
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

char* toString(nciStatus theStatus);