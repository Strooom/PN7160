            // case nciState::EnableCustomCommandsRfc:
            //     sendMessage(MsgTypeCommand, GroupIdProprietary, nci_PROPRIETARY_ACT_CMD);        // Send nci_PROPRIETARY_ACT_CMD to activate extra PN7150-nci features
            //     timerStart(10);                                                                  // we should get a RESPONSE within 10 ms, typically it takes 0.5ms
            //     state = nciState::EnableCustomCommandsWfr;                                       // move to next state, waiting for response
            //     break;

            // case nciState::EnableCustomCommandsWfr:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         bool isOk = isMessageType(MsgTypeResponse, GroupIdProprietary, nci_PROPRIETARY_ACT_RSP);        // Is the received Msg the correct type ?
            //         isOk      = isOk && (STATUS_OK == rxBuffer[3]);                                                 // Is the received Status code Status_OK ?

            //         if (isOk) {                             // if everything is OK...
            //             state = nciState::RfIdleCmd;        // ...move to the next state
            //         } else {                                // if not..
            //             state = nciState::Error;            // .. goto error state
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // time out waiting for response..
            //     }
            //     break;

            // case nciState::RfIdleCmd: {
            //     // After configuring, we are ready to go into Discovery, but we wait for the readerWriter application to give us this trigger
            //     // Or we can proceed into polling right away
            //     discover();
            //     // uint8_t payloadData[] = {4, NFC_A_PASSIVE_POLL_MODE, 0x01, NFC_B_PASSIVE_POLL_MODE, 0x01, NFC_F_PASSIVE_POLL_MODE, 0x01, NFC_15693_PASSIVE_POLL_MODE, 0x01};
            //     // sendMessage(MsgTypeCommand, GroupIdRfManagement, RF_DISCOVER_CMD, payloadData, 9);        //
            //     // setTimeOut(10);                                                                           // we should get a RESPONSE within 10 ms
            //     // theState = NciState::RfIdleWfr;                                                           // move to next state, waiting for Response
            // } break;

            // case nciState::RfIdleWfr:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         bool isOk = (4 == rxMessageLength);                                                              // Does the received Msg have the correct lenght ?
            //         isOk      = isOk && isMessageType(MsgTypeResponse, GroupIdRfManagement, RF_DISCOVER_RSP);        // Is the received Msg the correct type ?
            //         isOk      = isOk && (STATUS_OK == rxBuffer[3]);                                                  // Is the received Status code Status_OK ?
            //         if (isOk)                                                                                        // if everything is OK...
            //         {
            //             state = nciState::RfDiscovery;        // ...move to the next state
            //             timerStart(500);                      // set a timeout of 1 second. If it times out, it means no cards are present..
            //         } else                                    // if not..
            //         {
            //             state = nciState::Error;        // .. goto error state
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // time out waiting for response..
            //     }
            //     break;

            // case nciState::RfDiscovery:
            //     // TODO : if we have no NTF here, it means no cards are present and we can delete them from the list...
            //     // Here we don't check timeouts.. we can wait forever for a TAG/CARD to be presented..
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         if (isMessageType(MsgTypeNotification, GroupIdRfManagement, RF_INTF_ACTIVATED_NTF)) {
            //             // When a single tag/card is detected, the PN7150 will immediately activate it and send you this type of notification
            //             saveTag(RF_INTF_ACTIVATED_NTF);        // save properties of this Tag in the Tags array
            //             if (tagPresentStatus::noTagPresent == tagsStatus) {
            //                 tagsStatus = tagPresentStatus::newTagPresent;
            //             }
            //             state = nciState::RfPollActive;        // move to PollActive, and wait there for further commands..
            //         } else if (isMessageType(MsgTypeNotification, GroupIdRfManagement, RF_DISCOVER_NTF)) {
            //             // When multiple tags/cards are detected, the PN7150 will notify them all and wait for the DH to select one
            //             // The first card will have NotificationType == 2 and move the stateMachine to WaitForAllDiscoveries.
            //             // More notifications will come in that state
            //             saveTag(RF_DISCOVER_NTF);        // save properties of this Tag in the Tags array
            //             timerStart(25);                  // we should get more Notifications ubt set a timeout so we don't wait forever
            //             tagsStatus = tagPresentStatus::multipleTagsPresent;
            //             state      = nciState::RfWaitForAllDiscoveries;
            //         }
            //     } else if (isTimerExpired()) {
            //         tagsStatus = tagPresentStatus::noTagPresent;        // this means no card has been detected for xxx millisecond, so we can conclude that no cards are present
            //     }

            //     break;

            // case nciState::RfWaitForAllDiscoveries:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         if (isMessageType(MsgTypeNotification, GroupIdRfManagement, RF_DISCOVER_NTF)) {
            //             notificationType theNotificationType = (notificationType)rxBuffer[rxBuffer[6] + 7];        // notificationType comes in rxBuffer at the end, = 7 bytes + length of RF Technology Specific parameters which are in rxBuffer[6]
            //             switch (theNotificationType) {
            //                 case notificationType::lastNotification:
            //                 case notificationType::lastNotificationNfccLimit:
            //                     saveTag(RF_DISCOVER_NTF);        // save properties of this Tag in the Tags array
            //                     state = nciState::RfWaitForHostSelect;
            //                     break;

            //                 case notificationType::moreNotification:
            //                     timerStart(25);                  // we should get more Notifications, so set a timeout so we don't wait forever
            //                     saveTag(RF_DISCOVER_NTF);        // save properties of this Tag in the Tags array
            //                     break;

            //                 default:
            //                     break;
            //             }
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // We need a timeout here, in case the final RF_DISCOVER_NTF with Notification Type == 0 or 1 never comes...
            //     }
            //     break;

            // case nciState::RfWaitForHostSelect:
            //     // Multiple cards are present. We could select one and move into RfPollActive
            //     deActivate(NciRfDeAcivationMode::IdleMode);        //
            //     break;

            // case nciState::RfPollActive:
            //     // A card is present, so we can read/write data to it. We could also receive a notification that the card has been removed..
            //     deActivate(NciRfDeAcivationMode::IdleMode);        //
            //     break;

            // case nciState::RfDeActivate1Wfr:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         if (isMessageType(MsgTypeResponse, GroupIdRfManagement, RF_DEACTIVATE_RSP)) {
            //             state = nciState::RfIdleCmd;
            //         } else {
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // We need a timeout here, in case the RF_DEACTIVATE_RSP never comes...
            //     }
            //     break;

            // case nciState::RfDeActivate2Wfr:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         if (isMessageType(MsgTypeResponse, GroupIdRfManagement, RF_DEACTIVATE_RSP)) {
            //             timerStart(10);
            //             state = nciState::RfDeActivate2Wfn;
            //         } else {
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // We need a timeout here, in case the RF_DEACTIVATE_RSP never comes...
            //     }
            //     break;

            // case nciState::RfDeActivate2Wfn:
            //     if (PN7160Interface::hasMessage()) {
            //         getMessage();
            //         if (isMessageType(MsgTypeNotification, GroupIdRfManagement, RF_DEACTIVATE_NTF)) {
            //             state = nciState::RfIdleCmd;
            //         } else {
            //         }
            //     } else if (isTimerExpired()) {
            //         state = nciState::Error;        // We need a timeout here, in case the RF_DEACTIVATE_RSP never comes...
            //     }
            //     break;



// void nci::saveTag(uint8_t msgType) {
//     // Store the properties of detected TAGs in the Tag array.
//     // Tag info can come in two different nci messages : RF_DISCOVER_NTF and RF_INTF_ACTIVATED_NTF and the Tag properties are in slightly different location inside these messages

//     if (nmbrOfTags < maxNmbrTags) {
//         uint8_t offSet;        // Offset in the nci message where we can find the UniqueID
//         switch (msgType) {
//             case RF_INTF_ACTIVATED_NTF:
//                 offSet = 12;
//                 break;

//             case RF_DISCOVER_NTF:
//                 offSet = 9;
//                 break;

//             default:
//                 return;        // unknown type of msg sent here ?? we just ignore it..
//                 break;
//         }

//         uint8_t NfcId1Length = rxBuffer[offSet];
//         if (NfcId1Length > 10) {
//             NfcId1Length = 10;        // limit the length to 10, so in case of whatever error we don't write beyond the boundaries of the array
//         }
//         uint8_t newTagIndex = nmbrOfTags;        // index to the array item where we will store the info

//         theTags[newTagIndex].uniqueIdLength = NfcId1Length;           // copy the length of the unique ID, is 4, 7 or 10
//         for (uint8_t index = 0; index < NfcId1Length; index++)        // copy all bytes of the unique ID
//         {
//             theTags[newTagIndex].uniqueId[index] = rxBuffer[offSet + 1 + index];
//         }
//         theTags[newTagIndex].detectionTimestamp = millis();

//         nmbrOfTags++;        // one more tag in the array now
//     }
// }


// void nci::deActivate(NciRfDeAcivationMode theMode) {
//     nciState tmpState = getState();
//     switch (tmpState) {
//         case nciState::RfWaitForHostSelect: {
//             uint8_t payloadData[] = {(uint8_t)NciRfDeAcivationMode::IdleMode};
//             sendMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DEACTIVATE_CMD, payloadData, 1);
//             timerStart(10);
//             moveState(nciState::RfDeActivate1Wfr);
//         } break;

//         case nciState::RfPollActive: {
//             uint8_t payloadData[] = {(uint8_t)theMode};
//             sendMessage(messageType::Command, groupIdentifier::RfManagement, opcodeIdentifier::RF_DEACTIVATE_CMD, payloadData, 1);
//             timerStart(10);
//             moveState(nciState::RfDeActivate2Wfr);
//         } break;

//         default:
//             break;
//     }
// }



    //         if ((theMessageType == messageType::Command) && (groupID == groupIdentifier::Core) && (opCode == opcodeIdentifier::CORE_RESET_CMD)) {
    //         } else if ((theMessageType == messageType::Response) && (groupID == groupIdentifier::Core) && (opCode == opcodeIdentifier::CORE_RESET_RSP)) {
    //         } else if ((theMessageType == messageType::Notification) && (groupID == groupIdentifier::Core) && (opCode == opcodeIdentifier::CORE_RESET_NTF)) {
    //             if (MsgTypeData == theMessageType) {
    //                 printIndex(1);
    //                 logging::snprintfHex(msgBuffer[1]);
    //                 logging::snprintf(" : RFU\n");
    //             } else {
    //                 printIndex(1);
    //                 logging::snprintfHex(msgBuffer[1]);
    //                 logging::snprintf(" : opCode = ");
    //                 switch (groupID) {
    //                     case GroupIdCore:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_CMD:
    //                                         logging::snprintf("Core Reset Command\n");
    //                                         printLength(msgBuffer[2]);

    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         logging::snprintf(" : Configuration Status = ");
    //                                         printConfigurationStatus(msgBuffer[3]);
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     case CORE_INIT_CMD:
    //                                         logging::snprintf("Core Init Command\n");
    //                                         printLength(msgBuffer[2]);
    //                                         break;

    //                                     case CORE_SET_CONFIG_CMD:
    //                                         logging::snprintf("Core Set Config Command\n");
    //                                         break;

    //                                     case CORE_GET_CONFIG_CMD:
    //                                         logging::snprintf("Core Get Config Command\n");
    //                                         break;

    //                                     case CORE_CONN_CREATE_CMD:
    //                                         logging::snprintf("Core Connection Create Command\n");
    //                                         break;

    //                                     case CORE_CONN_CLOSE_CMD:
    //                                         logging::snprintf("Core Connection Close Command\n");
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_RSP:
    //                                         logging::snprintf("Core Reset Response\n");
    //                                         printLength(msgBuffer[2]);

    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         printStatus(msgBuffer[3]);
    //                                         logging::snprintf("\n");

    //                                         printIndex(4);
    //                                         logging::snprintfHex(msgBuffer[4]);
    //                                         logging::snprintf(" : NCI Version = V");
    //                                         logging::snprintf((uint8_t)((msgBuffer[4] & 0xF0) >> 4));
    //                                         logging::snprintf(".");
    //                                         logging::snprintf((uint8_t)(msgBuffer[4] & 0x0F));
    //                                         logging::snprintf("\n");

    //                                         printIndex(5);
    //                                         logging::snprintfHex(msgBuffer[5]);
    //                                         printConfigurationStatus(msgBuffer[5]);
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     case CORE_INIT_RSP:
    //                                         logging::snprintf("Core Init Response\n");
    //                                         printLength(msgBuffer[2]);

    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         printStatus(msgBuffer[3]);
    //                                         logging::snprintf("\n");

    //                                         printIndex(4);
    //                                         logging::snprintfHex(msgBuffer[4]);
    //                                         logging::snprintf(" : NFCC Features 1 = ");
    //                                         switch (msgBuffer[4] & 0x06) {
    //                                             case 0x00:
    //                                                 logging::snprintf("NFCC configured by DH only");
    //                                                 break;

    //                                             case 0x02:
    //                                                 logging::snprintf("NFCC configured by DH and other NFCEEs");
    //                                                 break;

    //                                             default:
    //                                                 // rest is RFU
    //                                                 break;
    //                                         }
    //                                         if (msgBuffer[4] & 0x01) {
    //                                             logging::snprintf(" | Discovery Frequency Supported");
    //                                         } else {
    //                                             logging::snprintf(" | Discovery Frequency Value Ignored");
    //                                         }

    //                                         printIndex(5);
    //                                         logging::snprintfHex(msgBuffer[5]);
    //                                         logging::snprintf(" | NFCC Features 2 = ");
    //                                         if (msgBuffer[5] & 0x08) {
    //                                             logging::snprintf("AID based routing Supported");
    //                                         } else {
    //                                             logging::snprintf("AID based routing NOT Supported");
    //                                         }
    //                                         if (msgBuffer[5] & 0x04) {
    //                                             logging::snprintf(" | Protocol based routing Supported");
    //                                         } else {
    //                                             logging::snprintf(" | Protocol based routin NOT Supported");
    //                                         }
    //                                         if (msgBuffer[5] & 0x02) {
    //                                             logging::snprintf(" | Technology based Routing Supported");
    //                                         } else {
    //                                             logging::snprintf(" | Technology based Routing NOT Supported");
    //                                         }
    //                                         logging::snprintf("\n");

    //                                         printIndex(6);
    //                                         logging::snprintfHex(msgBuffer[6]);
    //                                         logging::snprintf(" : NFCC Features 3 = ");
    //                                         if (msgBuffer[6] & 0x02) {
    //                                             logging::snprintf("Switched Off state Supported");
    //                                         } else {
    //                                             logging::snprintf("Switched Off state NOT Supported");
    //                                         }
    //                                         if (msgBuffer[6] & 0x01) {
    //                                             logging::snprintf(" | Battery Off state Supported");
    //                                         } else {
    //                                             logging::snprintf(" | Battery Off state NOT Supported");
    //                                         }
    //                                         logging::snprintf("\n");

    //                                         printIndex(7);
    //                                         logging::snprintfHex(msgBuffer[7]);
    //                                         logging::snprintf("NFCC Features 4 - TBC\n");

    //                                         printIndex(8);
    //                                         logging::snprintfHex(msgBuffer[8]);
    //                                         logging::snprintf(" : Number of Supported RF Interfaces = ");
    //                                         logging::snprintf(msgBuffer[8]);
    //                                         logging::snprintf("\n");

    //                                         for (uint8_t index = 0; index < msgBuffer[8]; index++) {
    //                                             printIndex(9 + index);
    //                                             logging::snprintfHex(msgBuffer[9 + index]);
    //                                             logging::snprintf(", RF Interface");
    //                                             logging::snprintf(index);
    //                                             logging::snprintf(" = ");
    //                                             printRfInterface(msgBuffer[9 + index]);
    //                                             logging::snprintf("\n");
    //                                         }

    //                                         printIndex(msgBuffer[8] + 9);
    //                                         logging::snprintfHex(msgBuffer[msgBuffer[8] + 9]);
    //                                         logging::snprintf(" : Max Logical Connections = ");
    //                                         logging::snprintf(msgBuffer[msgBuffer[8] + 9]);

    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     case CORE_SET_CONFIG_RSP:
    //                                         logging::snprintf("Core Set Config Response");
    //                                         break;

    //                                     case CORE_GET_CONFIG_RSP:
    //                                         logging::snprintf("Core Get Config Response");
    //                                         break;

    //                                     case CORE_CONN_CREATE_RSP:
    //                                         logging::snprintf("Core Connecton Create response");
    //                                         break;

    //                                     case CORE_CONN_CLOSE_RSP:
    //                                         logging::snprintf("Core Connection Close Response");
    //                                         break;

    //                                     default:
    //                                         // TODO : handle unknown opCode
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_NTF:
    //                                         logging::snprintf("Core Reset Notification");
    //                                         break;

    //                                     case CORE_CONN_CREDITS_NTF:
    //                                         logging::snprintf("Core Connection Credits Notification");
    //                                         break;

    //                                     case CORE_GENERIC_ERROR_NTF:
    //                                         logging::snprintf("Core Generic Error Notification");
    //                                         break;

    //                                     case CORE_INTERFACE_ERROR_NTF:
    //                                         logging::snprintf("Core Interface Error Notification");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 // TODO : handle unknown messageType
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdRfManagement:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case RF_DISCOVER_MAP_CMD:
    //                                         logging::snprintf("RF Discover Map Command");
    //                                         break;

    //                                     case RF_SET_LISTEN_MODE_ROUTING_CMD:
    //                                         logging::snprintf("RF Set Listen Mode Routing Command");
    //                                         break;

    //                                     case RF_GET_LISTEN_MODE_ROUTING_CMD:
    //                                         logging::snprintf("RF Get Listen Mode Routing Command");
    //                                         break;

    //                                     case RF_DISCOVER_CMD:
    //                                         logging::snprintf("RF Discover Command");
    //                                         break;

    //                                     case RF_DISCOVER_SELECT_CMD:
    //                                         logging::snprintf("RF Discover Select Command");
    //                                         break;

    //                                     case RF_DEACTIVATE_CMD:
    //                                         logging::snprintf("RF Deactivate Command");
    //                                         break;

    //                                     case RF_T3T_POLLING_CMD:
    //                                         logging::snprintf("RF T3T Polling Command");
    //                                         break;

    //                                     case RF_PARAMETER_UPDATE_CMD:
    //                                         logging::snprintf("RF Parameter Update Command");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case RF_DISCOVER_MAP_RSP:
    //                                         logging::snprintf("RF Discover Map Response");
    //                                         break;

    //                                     case RF_SET_LISTEN_MODE_ROUTING_RSP:
    //                                         logging::snprintf("RF Set Listen Mode Routing Response");
    //                                         break;

    //                                     case RF_GET_LISTEN_MODE_ROUTING_RSP:
    //                                         logging::snprintf("RF Get Lsiten Mode Routing Response");
    //                                         break;

    //                                     case RF_DISCOVER_RSP:
    //                                         logging::snprintf("RF Discover Response");
    //                                         printLength(msgBuffer[2]);

    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         printStatus(msgBuffer[3]);
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     case RF_DISCOVER_SELECT_RSP:
    //                                         logging::snprintf("RF Discover Select Response");
    //                                         break;

    //                                     case RF_DEACTIVATE_RSP:
    //                                         logging::snprintf("RF Deactivate Response\n");
    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         printStatus(msgBuffer[3]);
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     case RF_T3T_POLLING_RSP:
    //                                         logging::snprintf("RF T3T Polling Response");
    //                                         break;

    //                                     case RF_PARAMETER_UPDATE_RSP:
    //                                         logging::snprintf("RF Parameter Update Response");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case RF_GET_LISTEN_MODE_ROUTING_NTF:
    //                                         logging::snprintf("RF Get Listen Mode Routing Notification");
    //                                         break;

    //                                     case RF_DISCOVER_NTF:
    //                                         logging::snprintf("RF Discovery Notification\n");
    //                                         printLength(msgBuffer[2]);

    //                                         // RF Discovery ID - see NCI Spec V1.0 Table 61 and table 53
    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         logging::snprintf(" : RF Discovery ID = ");
    //                                         logging::snprintf(msgBuffer[3]);
    //                                         logging::snprintf("\n");

    //                                         // RF Protocol, table 98
    //                                         printIndex(4);
    //                                         logging::snprintfHex(msgBuffer[4]);
    //                                         logging::snprintf(" : RF Protocol = ");
    //                                         printRfProtocol(msgBuffer[4]);
    //                                         logging::snprintf("\n");

    //                                         // RF technology and Mode, table 96
    //                                         printIndex(5);
    //                                         logging::snprintfHex(msgBuffer[5]);
    //                                         logging::snprintf(" : RF Technology and Mode = ");
    //                                         printRfTechnologyAndMode(msgBuffer[5]);
    //                                         logging::snprintf("\n");

    //                                         // Length of RF Technology Specific Parameters'
    //                                         printIndex(6);
    //                                         logging::snprintfHex(msgBuffer[6]);
    //                                         logging::snprintf(" : Length of RF Technology Specific Parameters = ");
    //                                         logging::snprintf(msgBuffer[6]);
    //                                         logging::snprintfNow("\n");

    //                                         switch (msgBuffer[5]) {
    //                                             case NFC_A_PASSIVE_POLL_MODE:
    //                                                 printIndex(7, 8);
    //                                                 logging::snprintf("SENS_RES Response = ");
    //                                                 logging::snprintfHex(msgBuffer[7]);
    //                                                 logging::snprintf(" ");
    //                                                 logging::snprintfHex(msgBuffer[8]);
    //                                                 logging::snprintfNow("\n");

    //                                                 printIndex(9);
    //                                                 logging::snprintfHex(msgBuffer[9]);
    //                                                 logging::snprintf(" : NFC ID1 Length = ");
    //                                                 logging::snprintf(msgBuffer[9]);
    //                                                 logging::snprintfNow("\n");

    //                                                 printIndex(10, 10 + msgBuffer[9] - 1);
    //                                                 for (uint8_t index = 0; index < msgBuffer[9]; index++) {
    //                                                     logging::snprintfHex(msgBuffer[index + 10]);
    //                                                     logging::snprintf(" ");
    //                                                 }
    //                                                 logging::snprintfNow(" : NFC ID1\n");

    //                                                 break;
    //                                             case NFC_B_PASSIVE_POLL_MODE:
    //                                                 logging::snprintfNow(" : NFC_B_PASSIVE_POLL_MODE parameters - TBC\n");
    //                                                 break;
    //                                             case NFC_F_PASSIVE_POLL_MODE:
    //                                                 logging::snprintfNow(" : NFC_F_PASSIVE_POLL_MODE parameters - TBC\n");
    //                                                 break;
    //                                             default:
    //                                                 logging::snprintfNow(" : other RF technology Parameters - TBC\n");
    //                                                 break;
    //                                         }

    //                                         printIndex(7 + msgBuffer[6]);
    //                                         logging::snprintfHex(msgBuffer[7 + msgBuffer[6]]);
    //                                         logging::snprintf(" : Notification Type");
    //                                         logging::snprintfNow("\n\n");

    //                                         break;

    //                                     case RF_INTF_ACTIVATED_NTF:
    //                                         logging::snprintf("RF Interface Activated Notification\n");
    //                                         printLength(msgBuffer[2]);

    //                                         // RF Discovery ID - see NCI Spec V1.0 Table 61 and table 53
    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         logging::snprintf(" : RF Discovery ID = ");
    //                                         logging::snprintf(msgBuffer[3]);
    //                                         logging::snprintf("\n");

    //                                         // RF Interface, table 99
    //                                         printIndex(4);
    //                                         logging::snprintfHex(msgBuffer[4]);
    //                                         logging::snprintf(" : RF Interface = ");
    //                                         printRfInterface(msgBuffer[4]);
    //                                         logging::snprintf("\n");

    //                                         // RF Protocol, table 98
    //                                         printIndex(5);
    //                                         logging::snprintfHex(msgBuffer[5]);
    //                                         logging::snprintf(" : RF Protocol = ");
    //                                         printRfProtocol(msgBuffer[5]);
    //                                         logging::snprintf("\n");

    //                                         // Activation RF technology and Mode, table 96
    //                                         printIndex(6);
    //                                         logging::snprintfHex(msgBuffer[6]);
    //                                         logging::snprintf(" : RF Technology and Mode = ");
    //                                         printRfTechnologyAndMode(msgBuffer[6]);
    //                                         logging::snprintf("\n");

    //                                         printIndex(7);
    //                                         logging::snprintfHex(msgBuffer[7]);
    //                                         logging::snprintf(" : Max Data Packet Payload Size = ");
    //                                         logging::snprintf(msgBuffer[7]);
    //                                         logging::snprintf("\n");

    //                                         // table 14
    //                                         printIndex(8);
    //                                         logging::snprintfHex(msgBuffer[8]);
    //                                         logging::snprintf(" : Initial Number of Credits = ");
    //                                         logging::snprintf(msgBuffer[8]);
    //                                         logging::snprintf("\n");

    //                                         // RF Technology Specific Parameters, depending on the value of 'RF Technology and Mode'
    //                                         printIndex(9);
    //                                         logging::snprintfHex(msgBuffer[9]);
    //                                         logging::snprintf(" : Length of RF Technology Specific Parameters = ");
    //                                         logging::snprintf(msgBuffer[9]);
    //                                         logging::snprintfNow("\n");

    //                                         switch (msgBuffer[6]) {
    //                                             case NFC_A_PASSIVE_POLL_MODE:
    //                                                 printIndex(10, 11);
    //                                                 logging::snprintf("SENS_RES Response = ");
    //                                                 logging::snprintfHex(msgBuffer[10]);
    //                                                 logging::snprintf(" ");
    //                                                 logging::snprintfHex(msgBuffer[11]);
    //                                                 logging::snprintfNow("\n");

    //                                                 printIndex(12);
    //                                                 logging::snprintfHex(msgBuffer[12]);
    //                                                 logging::snprintf(" : NFC ID1 Length = ");
    //                                                 logging::snprintf(msgBuffer[12]);
    //                                                 logging::snprintfNow("\n");

    //                                                 printIndex(13, 13 + msgBuffer[12] - 1);
    //                                                 for (uint8_t index = 0; index < msgBuffer[12]; index++) {
    //                                                     logging::snprintfHex(msgBuffer[index + 13]);
    //                                                     logging::snprintf(" ");
    //                                                 }
    //                                                 logging::snprintfNow(" : NFC ID1\n");

    //                                                 break;
    //                                             case NFC_B_PASSIVE_POLL_MODE:
    //                                                 logging::snprintfNow(" : NFC_B_PASSIVE_POLL_MODE parameters - TBC\n");
    //                                                 break;
    //                                             case NFC_F_PASSIVE_POLL_MODE:
    //                                                 logging::snprintfNow(" : NFC_F_PASSIVE_POLL_MODE parameters - TBC\n");
    //                                                 break;
    //                                             default:
    //                                                 logging::snprintfNow(" : other RF technology Parameters - TBC\n");
    //                                                 break;
    //                                         }
    //                                         break;

    //                                     case RF_DEACTIVATE_NTF:
    //                                         logging::snprintf("RF Deactivate Notification\n");
    //                                         printLength(msgBuffer[2]);

    //                                         printIndex(3);
    //                                         logging::snprintfHex(msgBuffer[3]);
    //                                         printDeactivationType((NciRfDeAcivationMode)msgBuffer[3]);

    //                                         printIndex(4);
    //                                         logging::snprintfHex(msgBuffer[4]);
    //                                         printDeactivationReason(msgBuffer[4]);
    //                                         break;

    //                                     case RF_FIELD_INFO_NTF:
    //                                         logging::snprintf("RF Field Info Notification");
    //                                         break;

    //                                     case RF_T3T_POLLING_NTF:
    //                                         logging::snprintf("RF T3T Polling Notification");
    //                                         break;

    //                                     case RF_NFCEE_ACTION_NTF:
    //                                         logging::snprintf("RF NFCEE Action Notification");
    //                                         break;

    //                                     case RF_NFCEE_DISCOVERY_REQ_NTF:
    //                                         logging::snprintf("RF NFCEE Discovery Notification");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 // TODO : handle unknown messageType
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdNfceeManagement:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_CMD:
    //                                         logging::snprintf("NFCEE Discover Command");
    //                                         break;

    //                                     case NFCEE_MODE_SET_CMD:
    //                                         logging::snprintf("NFCEE Mode Set Command");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_RSP:
    //                                         logging::snprintf("NFCEE Discover Response");
    //                                         break;

    //                                     case NFCEE_MODE_SET_RSP:
    //                                         logging::snprintf("NFC Mode Set Response");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_NTF:
    //                                         logging::snprintf("NFCEE Discover Notification");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 // TODO : handle unknown messageType
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdProprietary:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case NCI_PROPRIETARY_ACT_CMD:
    //                                         logging::snprintf("Activate Proprietary Features Command");
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case NCI_PROPRIETARY_ACT_RSP:
    //                                         logging::snprintf("Activate Proprietary Features Response");
    //                                         logging::snprintf("\n");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 break;

    //                             default:
    //                                 // TODO : handle unknown messageType
    //                                 break;
    //                         }
    //                         break;

    //                     default:
    //                         // TODO : handle unknown groupId
    //                         break;
    //                 }
    //             }
    //             logging::snprintfNow("\n");
    //         } else {
    //             if (MsgTypeData == theMessageType) {
    //             } else {
    //                 switch (groupID) {
    //                     case GroupIdCore:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_CMD:
    //                                         logging::snprintf("Core Reset Command");
    //                                         break;

    //                                     case CORE_INIT_CMD:
    //                                         logging::snprintf("Core Init Command");
    //                                         break;

    //                                     case CORE_SET_CONFIG_CMD:
    //                                         logging::snprintf("Core Set Config Command");
    //                                         break;

    //                                     case CORE_GET_CONFIG_CMD:
    //                                         logging::snprintf("Core Get Config Command");
    //                                         break;

    //                                     case CORE_CONN_CREATE_CMD:
    //                                         logging::snprintf("Core Connection Create Command");
    //                                         break;

    //                                     case CORE_CONN_CLOSE_CMD:
    //                                         logging::snprintf("Core Connection Close Command");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_RSP:
    //                                         logging::snprintf("Core Reset Response");
    //                                         break;

    //                                     case CORE_INIT_RSP:
    //                                         logging::snprintf("Core Init Response");
    //                                         break;

    //                                     case CORE_SET_CONFIG_RSP:
    //                                         logging::snprintf("Core Set Config Response");
    //                                         break;

    //                                     case CORE_GET_CONFIG_RSP:
    //                                         logging::snprintf("Core Get Config Response");
    //                                         break;

    //                                     case CORE_CONN_CREATE_RSP:
    //                                         logging::snprintf("Core Connecton Create response");
    //                                         break;

    //                                     case CORE_CONN_CLOSE_RSP:
    //                                         logging::snprintf("Core Connection Close Response");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case CORE_RESET_NTF:
    //                                         logging::snprintf("Core Reset Notification");
    //                                         break;

    //                                     case CORE_CONN_CREDITS_NTF:
    //                                         logging::snprintf("Core Connection Credits Notification");
    //                                         break;

    //                                     case CORE_GENERIC_ERROR_NTF:
    //                                         logging::snprintf("Core Generic Error Notification");
    //                                         break;

    //                                     case CORE_INTERFACE_ERROR_NTF:
    //                                         logging::snprintf("Core Interface Error Notification");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 logging::snprintf("Unknown messageType : ");
    //                                 logging::snprintfHex(theMessageType >> 4);
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdRfManagement:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case RF_DISCOVER_MAP_CMD:
    //                                         logging::snprintf("RF Discover Map Command");
    //                                         break;

    //                                     case RF_SET_LISTEN_MODE_ROUTING_CMD:
    //                                         logging::snprintf("RF Set Listen Mode Routing Command");
    //                                         break;

    //                                     case RF_GET_LISTEN_MODE_ROUTING_CMD:
    //                                         logging::snprintf("RF Get Listen Mode Routing Command");
    //                                         break;

    //                                     case RF_DISCOVER_CMD:
    //                                         logging::snprintf("RF Discover Command");
    //                                         break;

    //                                     case RF_DISCOVER_SELECT_CMD:
    //                                         logging::snprintf("RF Discover Select Command");
    //                                         break;

    //                                     case RF_DEACTIVATE_CMD:
    //                                         logging::snprintf("RF Deactivate Command");
    //                                         break;

    //                                     case RF_T3T_POLLING_CMD:
    //                                         logging::snprintf("RF T3T Polling Command");
    //                                         break;

    //                                     case RF_PARAMETER_UPDATE_CMD:
    //                                         logging::snprintf("RF Parameter Update Command");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case RF_DISCOVER_MAP_RSP:
    //                                         logging::snprintf("RF Discover Map Response");
    //                                         break;

    //                                     case RF_SET_LISTEN_MODE_ROUTING_RSP:
    //                                         logging::snprintf("RF Set Listen Mode Routing Response");
    //                                         break;

    //                                     case RF_GET_LISTEN_MODE_ROUTING_RSP:
    //                                         logging::snprintf("RF Get Lsiten Mode Routing Response");
    //                                         break;

    //                                     case RF_DISCOVER_RSP:
    //                                         logging::snprintf("RF Discover Response");
    //                                         break;

    //                                     case RF_DISCOVER_SELECT_RSP:
    //                                         logging::snprintf("RF Discover Select Response");
    //                                         break;

    //                                     case RF_DEACTIVATE_RSP:
    //                                         logging::snprintf("RF Deactivate Response");
    //                                         break;

    //                                     case RF_T3T_POLLING_RSP:
    //                                         logging::snprintf("RF T3T Polling Response");
    //                                         break;

    //                                     case RF_PARAMETER_UPDATE_RSP:
    //                                         logging::snprintf("RF Parameter Update Response");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case RF_GET_LISTEN_MODE_ROUTING_NTF:
    //                                         logging::snprintf("RF Get Listen Mode Routing Notification");
    //                                         break;

    //                                     case RF_DISCOVER_NTF:
    //                                         logging::snprintf("RF Discover Notification");
    //                                         break;

    //                                     case RF_INTF_ACTIVATED_NTF:
    //                                         logging::snprintf("RF Interface Activated Notification");
    //                                         break;

    //                                     case RF_DEACTIVATE_NTF:
    //                                         logging::snprintf("RF Deactivate Notification");
    //                                         break;

    //                                     case RF_FIELD_INFO_NTF:
    //                                         logging::snprintf("RF Field Info Notification");
    //                                         break;

    //                                     case RF_T3T_POLLING_NTF:
    //                                         logging::snprintf("RF T3T Polling Notification");
    //                                         break;

    //                                     case RF_NFCEE_ACTION_NTF:
    //                                         logging::snprintf("RF NFCEE Action Notification");
    //                                         break;

    //                                     case RF_NFCEE_DISCOVERY_REQ_NTF:
    //                                         logging::snprintf("RF NFCEE Discovery Notification");
    //                                         break;

    //                                     default:
    //                                         logging::snprintf("Unknown OpCode : ");
    //                                         logging::snprintfHex(opCode);
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 logging::snprintf("Unknown messageType : ");
    //                                 logging::snprintfHex(theMessageType >> 4);
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdNfceeManagement:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_CMD:
    //                                         logging::snprintf("NFCEE Discover Command");
    //                                         break;

    //                                     case NFCEE_MODE_SET_CMD:
    //                                         logging::snprintf("NFCEE Mode Set Command");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_RSP:
    //                                         logging::snprintf("NFCEE Discover Response");
    //                                         break;

    //                                     case NFCEE_MODE_SET_RSP:
    //                                         logging::snprintf("NFC Mode Set Response");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 switch (opCode) {
    //                                     case NFCEE_DISCOVER_NTF:
    //                                         logging::snprintf("NFCEE Discover Notification");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             default:
    //                                 logging::snprintf("Unknown messageType : ");
    //                                 logging::snprintfHex(theMessageType >> 4);
    //                                 break;
    //                         }
    //                         break;

    //                     case GroupIdProprietary:
    //                         switch (theMessageType) {
    //                             case MsgTypeCommand:
    //                                 switch (opCode) {
    //                                     case NCI_PROPRIETARY_ACT_CMD:
    //                                         logging::snprintf("Activate Proprietary Features Command");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeResponse:
    //                                 switch (opCode) {
    //                                     case NCI_PROPRIETARY_ACT_RSP:
    //                                         logging::snprintf("Activate Proprietary Features Response");
    //                                         break;

    //                                     default:
    //                                         break;
    //                                 }
    //                                 break;

    //                             case MsgTypeNotification:
    //                                 break;

    //                             default:
    //                                 // TODO : handle unknown messageType
    //                                 break;
    //                         }
    //                         break;

    //                     default:
    //                         logging::snprintf("Unknown groupID : ");
    //                         logging::snprintfHex(groupID);
    //                         break;
    //                 }
    //             }
    //             logging::snprintfNow("\n\n");
    //         }



    // void nciDecoder::printRfInterface(uint8_t theInterfaceCode) {
    //     switch (theInterfaceCode) {
    //         case 0x00:
    //             logging::snprintf("NFCEE Direct RF Interface");
    //             break;
    //         case 0x01:
    //             logging::snprintf("Frame RF Interface");
    //             break;
    //         case 0x02:
    //             logging::snprintf("ISO-DEP RF Interface");
    //             break;
    //         case 0x03:
    //             logging::snprintf("NFC-DEP RF Interface");
    //             break;
    //         default:
    //             logging::snprintf("Other RF Interface");
    //             break;
    //     }
    // }

    // void nciDecoder::printRfTechnologyAndMode(uint8_t theMode) {
    //     switch (theMode) {
    //         case NFC_A_PASSIVE_POLL_MODE:
    //             logging::snprintf("NFC_A_PASSIVE_POLL_MODE");
    //             break;
    //         case NFC_B_PASSIVE_POLL_MODE:
    //             logging::snprintf("NFC_B_PASSIVE_POLL_MODE");
    //             break;
    //         case NFC_F_PASSIVE_POLL_MODE:
    //             logging::snprintf("NFC_F_PASSIVE_POLL_MODE");
    //             break;
    //         default:
    //             logging::snprintf("Other Technology, TBC");
    //             break;
    //     }
    // }

    // void nciDecoder::printRfProtocol(uint8_t theProtocol) {
    //     switch (theProtocol) {
    //         case 0x01:
    //             logging::snprintf("T1T");
    //             break;
    //         case 0x02:
    //             logging::snprintf("T2T");
    //             break;
    //         case 0x03:
    //             logging::snprintf("T3T");
    //             break;
    //         case 0x04:
    //             logging::snprintf("ISO_DEP");
    //             break;
    //         case 0x05:
    //             logging::snprintf("NFC_DEP");
    //             break;
    //         default:
    //             logging::snprintf("Other protocol, Proprietary or RFU");
    //             break;
    //     }
    // }

    // void nciDecoder::printState(NciState theState) {
    //     switch (theState) {
    //         case NciState::HwResetRfc:
    //             logging::snprintf("HwResetRfc");
    //             break;

    //         case NciState::HwResetWfr:
    //             logging::snprintf("HwResetWfr");
    //             break;

    //         case NciState::SwResetRfc:
    //             logging::snprintf("SwResetRfc");
    //             break;

    //         case NciState::SwResetWfr:
    //             logging::snprintf("SwResetWfr");
    //             break;

    //         case NciState::EnableCustomCommandsRfc:
    //             logging::snprintf("EnableCustomCommandsRfc");
    //             break;

    //         case NciState::EnableCustomCommandsWfr:
    //             logging::snprintf("EnableCustomCommandsWfr");
    //             break;

    //         case NciState::RfIdleCmd:
    //             logging::snprintf("RfIdleCmd");
    //             break;

    //         case NciState::RfIdleWfr:
    //             logging::snprintf("RfIdleWfr");
    //             break;

    //         case NciState::RfGoToDiscoveryWfr:
    //             logging::snprintf("RfGoToDiscoveryWfr");
    //             break;

    //         case NciState::RfDiscovery:
    //             logging::snprintf("RfDiscovery");
    //             break;

    //         case NciState::RfPollActive:
    //             logging::snprintf("RfPollActive");
    //             break;

    //         case NciState::Error:
    //             logging::snprintf("Error");
    //             break;

    //         default:
    //             logging::snprintf("Unknown State : ");
    //             logging::snprintfHex((uint8_t)theState);
    //             break;
    //     }
    //     // logging::snprintf("\n");
    // }

    // void nciDecoder::printDeactivationType(NciRfDeAcivationMode theMode) {
    //     logging::snprintf(" : DeActivation Type : ");
    //     switch (theMode)        // Deactivation Type
    //     {
    //         case NciRfDeAcivationMode::IdleMode:
    //             logging::snprintf("Idle Mode");
    //             break;
    //         case NciRfDeAcivationMode::SleepMode:
    //             logging::snprintf("Sleep Mode");
    //             break;
    //         case NciRfDeAcivationMode::Sleep_AFMode:
    //             logging::snprintf("Slaap AF Mode");
    //             break;
    //         case NciRfDeAcivationMode::Discovery:
    //             logging::snprintf("Discovery");
    //             break;
    //         default:
    //             logging::snprintf("Unknown : ");
    //             logging::snprintfHex((uint8_t)theMode);
    //             break;
    //     }
    //     logging::snprintf("\n");
    // }

    // void nciDecoder::printDeactivationReason(uint8_t theReason) {
    //     logging::snprintf(" : DeActivation Reason : ");
    //     switch (theReason)        // Deactivation Reason
    //     {
    //         case DH_Request:
    //             logging::snprintf("DeviceHost Request");
    //             break;
    //         case Endpoint_Request:
    //             logging::snprintf("EndPoint Request");
    //             break;
    //         case RF_Link_Loss:
    //             logging::snprintf("RF Link Loss");
    //             break;
    //         case NFC_B_Bad_AFI:
    //             logging::snprintf("NFC-B Bad AFI");
    //             break;
    //         default:
    //             logging::snprintf("Unknown : ");
    //             logging::snprintfHex(theReason);
    //             break;
    //     }
    //     logging::snprintf("\n");
    // }


    //     static void printIndex(const uint32_t thevalue);
    // static void printIndex(const uint32_t begin, const uint32_t end);
    // static void printStatus(const uint8_t statusCode);
    // static void printConfigurationStatus(const uint8_t theStatus);
    // static void printLength(const uint8_t theLength);
    // static void printRfInterface(const uint8_t theInterfaceCode);
    // static void printRfTechnologyAndMode(const uint8_t theMode);
    // static void printRfProtocol(const uint8_t theProtocol);
    // static void printState(const nciState theState);
    // static void printDeactivationType(const NciRfDeAcivationMode theMode);
    // static void printDeactivationReason(const uint8_t theReason);


    // RF_DISCOVER_MAP_CMD = 0x00,
// RF_DISCOVER_MAP_RSP = 0x00,

// RF_SET_LISTEN_MODE_ROUTING_CMD = 0x01

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
