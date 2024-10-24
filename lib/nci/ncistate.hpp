#pragma once

#include <stdint.h>

enum class nciState : uint8_t
// Note :	Rfc means "Ready For Command" : a state where you would (typically) send an NCI command from the DH to the NFC
//			Wfr means "Waiting For Response" : a state where you would wait for a response from the NFC to the DH
//			Wfn means "Waiting For Notification" : a state where you would wait for a notification from the NFC to the DH
{
    HwResetRfc,                     // you start in this state after a hardware reset of the PN7160, then you send the CORE_RESET_CMD
    HwResetWfr,                     // waiting for CORE_RESET_RSP
    HwResetWfn,                     // waiting for CORE_RESET_NTF
    SwResetRfc,                     // send CORE_INIT_CMD
    SwResetWfr,                     // waiting for CORE_INIT_RSP
    EnableCustomCommandsRfc,        // Enabling PN7150-extensions
    EnableCustomCommandsWfr,        // waiting for response/confirmation
    RfIdleCmd,                      // Core initialized, now waiting for RF configuration commands
    RfIdleWfr,
    RfGoToDiscoveryWfr,
    RfDiscovery,                    // polling / detecting cards/tags
    RfWaitForAllDiscoveries,        // busy enumerating multiple cards/tags being detected
    RfWaitForHostSelect,            // done detecting multiple cards/tags, waiting for the DH to select one
    RfPollActive,                   // detected 1 card/tag, and activated it for reading/writing

    RfDeActivate1Wfr,        // waiting for deactivation response, no notification will come (dactivation in RfWaitForHostSelect)
    RfDeActivate2Wfr,        // waiting for deactivation response, additionally a notification will come (deactivation in RfPollActive)
    RfDeActivate2Wfn,        // waiting for deactivation notifiation
    Error,
    End
};
