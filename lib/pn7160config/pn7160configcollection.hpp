#pragma once

#include <stdint.h>
#include "pn7160config.hpp"

class pn7160ConfigCollection {
 
  public:
    static constexpr uint32_t nmbrOfConfigs{2};
    static const pn7160Config configs[nmbrOfConfigs];
    static uint32_t activeConfig;
};