#pragma once

#include <stdint.h>

class pn7160config {
  public:
    static constexpr uint32_t maxLenght{32};
    const uint16_t tag;
    const uint8_t length;
    const uint8_t data[maxLenght];

  private:

  friend class pn7160configcollection;
};