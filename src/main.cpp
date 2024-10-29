#include <Arduino.h>
#include <logging.hpp>
#include <i2c_t3.h>
#include <pn7160interface.hpp>
#include <nci.hpp>

void setup() {
    logging::initialize();
    logging::enable(logging::source::stateChanges);
    logging::enable(logging::source::tagEvents);
    logging::enable(logging::source::criticalError);
    logging::enable(logging::destination::uart1);

    Wire.begin();

    PN7160Interface::initialize(2, 3, 0x28);
    Serial.println("initialized"); // This line is only needed to make the cloud-build work : https://forum.pjrc.com/index.php?threads/undefined-reference-to-_write.71420/
}

void loop() {
    nci::run();
}
