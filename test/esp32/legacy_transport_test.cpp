#include "pn7160interface/pn7160interface.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <algorithm>
#include <cassert>
#include <cstdio>

int main() {
    PN7160Interface::initialize(3, 4, 0x28);
    assert(pinValues[4] == HIGH);
    PN7160Interface::setVenPin(false);
    assert(pinValues[4] == LOW);
    PN7160Interface::setVenPin(true);
    assert(PN7160Interface::wakeUp());
    const uint8_t command[] = {0x20, 0, 1, 0};
    assert(PN7160Interface::write(command, sizeof(command)) == 0);
    assert(std::equal(Wire.transmitted.begin(), Wire.transmitted.end(), command));
    assert(!PN7160Interface::hasMessage());
    pinValues[3] = HIGH;
    Wire.responses = {{0x40, 0, 1}, {0}};
    uint8_t response[258]{};
    assert(PN7160Interface::read(response) == 4);
    assert(response[0] == 0x40 && response[2] == 1 && response[3] == 0);
    std::puts("PASS: unchanged RP2040 transport branch and initialization API.");
}
