#pragma once

class singleShotTimer {
  public:
    void start(unsigned long);                  // re-start a new timeout
    void startOrContinue(unsigned long);        // start a new timeout or continue if already started
    void stop();                                // disable before expiring
    bool isRunning() const;                     // is the timer running or stopped
    bool isExpired();                           // check if timer has expired, if so stops it
    bool isExpiredAndContinue() const;          // check if timer has expired but does not stop it..

    unsigned long startTime() const;            // get timer startTime, in milliseconds
    unsigned long duration() const;             // get timer duration, in milliseconds
    unsigned long timePassed() const;           // time from start to now, in milliseconds
    unsigned long timeRemaining() const;        // time from now to expiration, in milliseconds

#ifdef unittesting
  private:
#endif
    bool timerIsRunning{false};
    unsigned long timerStartTime{0};
    unsigned long timerDuration{0};
#ifdef generic
    static unsigned long mockMillis;
#endif
};
