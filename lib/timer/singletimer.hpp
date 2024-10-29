#pragma once

class singleTimer {
  public:
    void start(unsigned long);                  // re-start a new timeout
    void startOrContinue(unsigned long);        // start a new timeout or continue if already started
    bool expired();                             // check if timer has expired, if so stops it
    bool expiredAndContinue() const;            // check if timer has expired but does not stop it..
    void stop();                                // disable before expiring
    bool isRunning() const;                           // is the timer running or stopped
    unsigned long value() const;                // time from start to now, in milliseconds
    unsigned long duration() const;             // get timer duration, in milliseconds

#ifdef unittesting
  private:
#endif
    bool timerIsRunning{false};
    unsigned long startTime;
    unsigned long timerDuration;
#ifdef generic
    static unsigned long mockMillis;
#endif
};
