#include <singletimer.hpp>
#ifndef generic
#include <Arduino.h>
#else
unsigned long singleTimer::mockMillis{0};
#endif

bool singleTimer::expired() {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        if (now - startTime > timerDuration) {
            timerIsRunning = false;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool singleTimer::expiredAndContinue() const {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        if (now - startTime > timerDuration) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void singleTimer::start(unsigned long theTimerDuration) {
    unsigned long now{0};
#ifndef generic
    now = millis();
#else
    now = mockMillis;
#endif
    startTime      = now;
    timerDuration  = theTimerDuration;
    timerIsRunning = true;
}

void singleTimer::startOrContinue(unsigned long theTimerDuration) {
    if (!timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        startTime      = now;
        timerDuration  = theTimerDuration;
        timerIsRunning = true;
    }
}

void singleTimer::stop() {
    timerIsRunning = false;
}

bool singleTimer::isRunning() const{
    return timerIsRunning;
}

unsigned long singleTimer::value() const {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        return (now - startTime);
    } else {
        return 0;
    }
}

unsigned long singleTimer::duration() const {
    return timerDuration;
}
