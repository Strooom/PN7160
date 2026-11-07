#pragma once
#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>
struct TwoWire
{
    bool beginResult = true;
    bool bufferResult = true;
    bool shortWrite = false;
    int unreadableAt = -1;
    size_t bufferSize = 0;
    uint16_t timeout = 0;
    int sda = -1, scl = -1;
    uint32_t clock = 0;
    int transmissions = 0;
    uint8_t lastAddress = 0;
    std::deque<uint8_t> status;
    std::deque<std::vector<uint8_t>> responses;
    std::vector<size_t> requests;
    std::vector<uint8_t> received, transmitted;
    size_t position = 0;
    size_t setBufferSize(size_t size) { bufferSize = size; return bufferResult ? size : 0; }
    bool begin(int a, int b, uint32_t frequency) { sda = a; scl = b; clock = frequency; return beginResult; }
    void begin() { begin(-1, -1, 100000); }
    void setTimeOut(uint16_t value) { timeout = value; }
    void beginTransmission(uint8_t address) { lastAddress = address; ++transmissions; }
    uint8_t endTransmission()
    {
        if (status.empty()) return 0;
        auto value = status.front(); status.pop_front(); return value;
    }
    size_t write(const uint8_t *data, size_t size)
    {
        transmitted.assign(data, data + size);
        return shortWrite ? size - 1 : size;
    }
    size_t requestFrom(uint8_t address, size_t size)
    {
        lastAddress = address; requests.push_back(size); position = 0;
        if (responses.empty()) { received.clear(); return 0; }
        received = responses.front(); responses.pop_front(); return received.size();
    }
    int read()
    {
        if (position == static_cast<size_t>(unreadableAt) || position >= received.size()) return -1;
        return received[position++];
    }
};
inline TwoWire Wire;
