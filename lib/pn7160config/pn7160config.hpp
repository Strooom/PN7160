#pragma once

#include <stdint.h>

class pn7160Config {
  public:
    static constexpr uint32_t maxLength{32};
    const uint16_t tag;
    const uint8_t length;
    const uint8_t *data;

    pn7160Config(const uint16_t tagInitValue, const uint8_t lengthInitValue, const uint8_t *dataInitPtr) : tag(tagInitValue), length(lengthInitValue), data(dataInitPtr) {}

  private:
    friend class pn7160ConfigCollection;
};