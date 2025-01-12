#include <Arduino.h>
#include <logging.hpp>
#include <pn7160interface.hpp>
#include <nci.hpp>

#ifdef PN7160_RP2040
#include <Wire.h>
#else
#include <i2c_t3.h>
#endif

void setup() {
    logging::initialize();

    #ifdef PN7160_RP2040
    delay(1000); // delay required to get debug serial ready
    #endif

    logging::enable(logging::source::criticalError);
    logging::enable(logging::source::nciMessages);
    logging::enable(logging::source::stateChanges);
    logging::enable(logging::source::tagEvents);
    Serial.println("PN7160 driver demo");        // This line is only needed to make the cloud-build work : https://forum.pjrc.com/index.php?threads/undefined-reference-to-_write.71420/

    Wire.setSDA(20);
    Wire.setSCL(21);
    Wire.begin();

    PN7160Interface::initialize(2, 3, 0x28);
}

void loop() {
    nci::run();
    tagStatus currentTagStatus = nci::getTagStatus();
    switch (currentTagStatus) {
        case tagStatus::foundNew:
            logging::snprintf(logging::source::tagEvents, "New tag detected : ");
            nci::tagData.dump();
            break;
        case tagStatus::removed:
            logging::snprintf(logging::source::tagEvents, "Tag removed\n");
            break;
        default:
            break;
    }
}
