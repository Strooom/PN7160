#include "intervaltimer.hpp"
#ifndef generic
#include <Arduino.h>
#else
unsigned long intervalTimer::mockMillis{0};
#endif

void intervalTimer::set(unsigned long interval) {
    timerDuration = interval;
}

bool intervalTimer::isExpired() {
    unsigned long now;
#ifndef generic
    now = millis();
#else
    now = mockMillis;
#endif

    if (timerIsRunning) {
        if (now - timerStartTime > timerDuration) {
            timerStartTime += timerDuration;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void intervalTimer::start() {
    if (timerDuration > 0) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        timerStartTime = now;
        timerIsRunning = true;
    }
}

void intervalTimer::startOrContinue() {
    if (!timerIsRunning && (timerDuration > 0)) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        timerStartTime = now;
        timerIsRunning = true;
    }
}

void intervalTimer::start(unsigned long interval) {
    set(interval);
    start();
}

void intervalTimer::stop() {
    timerIsRunning = false;
}

bool intervalTimer::isRunning() const {
    return timerIsRunning;
}

unsigned long intervalTimer::timePassed() const {
    if (timerIsRunning) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        return (now - timerStartTime);
    } else {
        return 0;
    }
}

unsigned long intervalTimer::duration() const {
    return timerDuration;
}

unsigned long intervalTimer::timeRemaining() const {
    if (timerIsRunning) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        return ((timerStartTime + timerDuration) - now);
    } else {
        return 0;
    }
}

unsigned long intervalTimer::startTime() const {
    return timerStartTime;
}
