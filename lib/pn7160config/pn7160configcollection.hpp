#pragma once

#include <stdint.h>
#include <pn7160config.hpp>

class pn7160configcollection {
  public:
    static constexpr uint32_t nmbrOfConfigs{2};
    static uint32_t activeConfig;

    pn7160config configs[nmbrOfConfigs]{
        pn7160config{0x0000, 0x02, {0xF4, 0x01}},                                                              // sets TOTAL_DURATION to 500 ms
        pn7160config{0xA00E, 0x0B, {0x11, 0x01, 0xC1, 0xB1, 0x00, 0xDA, 0x1E, 0x14, 0x00, 0xD0, 0x0C}},        // Power Management Config PMU_CFG, Selects CFG1
    };
};