#include <Arduino.h>
#include <logging.hpp>
#include <i2c_t3.h>
#include <pn7160interface.hpp>
#include <nci.hpp>

void setup() {
    logging::initialize();
    logging::enable(logging::destination::uart1);
    logging::disable(logging::source::stateChanges);
    logging::disable(logging::source::nciMessages);
    logging::enable(logging::source::tagEvents);
    logging::enable(logging::source::criticalError);
    Serial.println("PN7160 driver demo");        // This line is only needed to make the cloud-build work : https://forum.pjrc.com/index.php?threads/undefined-reference-to-_write.71420/

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
