#pragma once

class intervalTimer {
  public:
    void start();
    void startOrContinue();                // start the intervalTimer if not yet running, do nothing if it already runs
    void start(unsigned long);             // set a new interval time and start the intervaltimer
    void set(unsigned long);               // set a new interval time
    bool expired();                        // check if intervalTimer has expired
    void stop();                           // disable before expiring
    bool isRunning() const;                // is the timer running or stopped
    unsigned long value() const;           // time passed in current interval, in milliseconds
    unsigned long interval() const;        // current interval, in milliseconds

#ifndef unitTesting
  private:
#endif
    bool timerIsRunning{false};
    unsigned long intervalStartTime{0};
    unsigned long intervalDuration;
#ifdef generic
    static unsigned long mockMillis;        // mock millis() function for unit testing
#endif
};
