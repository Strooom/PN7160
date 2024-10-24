#include <Arduino.h>
#include <logging.hpp>
#include <i2c_t3.h>
#include <pn7160interface.hpp>
//#include <nci.hpp>

void setup() {
    logging::initialize();
    logging::enable(logging::destination::uart1);
    Wire.begin();
    PN7160Interface::initialize(2, 3, 0x28);
    PN7160Interface::reset();
}

void loop() {
//    NCI::run();
}
