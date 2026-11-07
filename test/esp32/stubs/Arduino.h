#pragma once
#include <cassert>
#include <cstdint>
constexpr int INPUT = 0;
constexpr int OUTPUT = 1;
constexpr int LOW = 0;
constexpr int HIGH = 1;
inline int gpioCalls = 0;
inline int pinValues[64] = {};
inline uint32_t delayMs = 0;
inline void pinMode(int pin, int) { assert(pin >= 0 && pin < 64); ++gpioCalls; }
inline void digitalWrite(int pin, int value) { assert(pin >= 0 && pin < 64); ++gpioCalls; pinValues[pin] = value; }
inline int digitalRead(int pin) { assert(pin >= 0 && pin < 64); return pinValues[pin]; }
inline void delay(uint32_t value) { delayMs += value; }

inline unsigned long millis() { return delayMs; }
struct TestSerial
{
    void begin(uint32_t) {}
    void write(const char *, uint32_t) {}
};
inline TestSerial Serial;
