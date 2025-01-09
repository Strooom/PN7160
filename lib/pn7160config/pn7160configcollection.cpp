#include <pn7160configcollection.hpp>

uint32_t pn7160configcollection::activeConfig{0};
const pn7160config pn7160configcollection::configs[pn7160configcollection::nmbrOfConfigs] =
    {
        pn7160config{0x0000, {0xF4, 0x01}},                                                              // sets TOTAL_DURATION to 500 ms
        pn7160config{0xA00E, {0x11, 0x01, 0xC1, 0xB1, 0x00, 0xDA, 0x1E, 0x14, 0x00, 0xD0, 0x0C}},        // Power Management Config PMU_CFG, Selects CFG1
};
