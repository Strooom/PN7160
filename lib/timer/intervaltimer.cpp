#include <intervaltimer.hpp>
#ifndef generic
#include <Arduino.h>
#endif


intervalTimer::intervalTimer() : timerIsRunning{false} {
}

void intervalTimer::set(unsigned long interval) {
    intervalDuration = interval;
}

bool intervalTimer::expired() {
    unsigned long now;
#ifndef generic
    now = millis();
#else
    now = mockMillis;
#endif

    if (timerIsRunning) {
        if (now - intervalStartTime > intervalDuration) {
            intervalStartTime += intervalDuration;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void intervalTimer::start() {
    if (intervalDuration > 0) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        intervalStartTime = now;
        timerIsRunning    = true;
    }
}

void intervalTimer::startOrContinue() {
    if (!timerIsRunning && (intervalDuration > 0)) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        intervalStartTime = now;
        timerIsRunning    = true;
    }
}

void intervalTimer::start(unsigned long interval) {
    intervalDuration = interval;
    unsigned long now;
#ifndef generic
    now = millis();
#else
    now = mockMillis;
#endif

    intervalStartTime = now;
    timerIsRunning    = true;
}

void intervalTimer::stop() {
    timerIsRunning = false;
}

bool intervalTimer::isRunning() {
    return timerIsRunning;
}

unsigned long intervalTimer::value() const {
    if (timerIsRunning) {
        unsigned long now;
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif

        return (now - intervalStartTime);
    } else {
        return 0;
    }
}

unsigned long intervalTimer::interval() const {
    return intervalDuration;
}
