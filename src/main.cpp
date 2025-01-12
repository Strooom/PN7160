#include <Arduino.h>
#include <logging.hpp>
#include <pn7160interface.hpp>
#include <nci.hpp>

#ifdef PN7160_RP2040
#include <Wire.h>
#define I2C_SDA_PIN 20
#define I2C_SCL_PIN 21
#define IRQ_PIN 16
#define VEN_PIN 17
#else
#include <i2c_t3.h>
#define I2C_SDA_PIN 18
#define I2C_SCL_PIN 19
#define IRQ_PIN 2
#define VEN_PIN 3
#endif

void setup() {
    logging::initialize();
    logging::enable(logging::destination::destUart1);

    #ifdef PN7160_RP2040
    delay(1000); // delay required to get debug serial ready
    #endif

    logging::enable(logging::source::criticalError);
    logging::enable(logging::source::nciMessages);
    logging::enable(logging::source::stateChanges);
    logging::enable(logging::source::tagEvents);
    Serial.println("PN7160 driver demo");        // This line is only needed to make the cloud-build work : https://forum.pjrc.com/index.php?threads/undefined-reference-to-_write.71420/

    Wire.setSDA(I2C_SDA_PIN);
    Wire.setSCL(I2C_SCL_PIN);
    Wire.begin();

    PN7160Interface::initialize(IRQ_PIN, VEN_PIN, 0x28);
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
