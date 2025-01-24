#include "singletimer.hpp"
#ifndef generic
#include <Arduino.h>
#else
unsigned long singleShotTimer::mockMillis{0};
#endif

bool singleShotTimer::isExpired() {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        if (now - timerStartTime > timerDuration) {
            timerIsRunning = false;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool singleShotTimer::isExpiredAndContinue() const {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        if (now - timerStartTime > timerDuration) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void singleShotTimer::start(unsigned long theTimerDuration) {
    unsigned long now{0};
#ifndef generic
    now = millis();
#else
    now = mockMillis;
#endif
    timerStartTime = now;
    timerDuration  = theTimerDuration;
    timerIsRunning = true;
}

void singleShotTimer::startOrContinue(unsigned long theTimerDuration) {
    if (!timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        timerStartTime = now;
        timerDuration  = theTimerDuration;
        timerIsRunning = true;
    }
}

void singleShotTimer::stop() {
    timerIsRunning = false;
}

bool singleShotTimer::isRunning() const {
    return timerIsRunning;
}

unsigned long singleShotTimer::timePassed() const {
    if (timerIsRunning) {
        unsigned long now{0};
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

unsigned long singleShotTimer::timeRemaining() const {
    if (timerIsRunning) {
        unsigned long now{0};
#ifndef generic
        now = millis();
#else
        now = mockMillis;
#endif
        return (timerDuration - (now - timerStartTime));
    } else {
        return 0;
    }
}

unsigned long singleShotTimer::duration() const {
    return timerDuration;
}

unsigned long singleShotTimer::startTime() const {
    return timerStartTime;
}