#pragma once
#include <stdint.h>

class counter {
  public:
    explicit counter(uint32_t maxCount);
    uint32_t value() const;
    bool expired() const;
    void increment();
    void reset();

  private:
    uint32_t count;
    uint32_t maxCount;
};
