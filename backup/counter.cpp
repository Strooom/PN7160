#include <runtimer.hpp>

counter::counter(uint32_t theMaxCount) : maxCount{theMaxCount} {
}

uint32_t counter::value() const {
    return count;
}
bool counter::expired() const {
    return (count >= maxCount);
}
void counter::increment() {
    count++;
}
void counter::reset() {
    count = 0;
}
