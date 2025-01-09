#pragma once

#include <stdint.h>
#include <algorithm>
#include <initializer_list>

class pn7160config {
  public:
    static constexpr uint32_t maxLenght{32};
    const uint16_t tag;
    const uint8_t length;
    const uint8_t data[maxLenght];

    pn7160config(uint16_t t, std::initializer_list<uint8_t> initData) : tag(t), length(initData.size()), data{0} {
        std::copy(initData.begin(), initData.end(), const_cast<uint8_t*>(data));
    }

  private:
    friend class pn7160configcollection;
};