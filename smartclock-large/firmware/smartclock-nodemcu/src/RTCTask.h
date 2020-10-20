#ifndef RTCTask_h
#define RTCTask_h

#include <Arduino.h>
#include <Scheduler.h>

class RTCTask : public Task
{

private:
    int8_t _rtc_enabled;
    int wait_time;
    int cycles;

protected:
    void setup();
    void loop();

public:
    int Y, M, D, h, m, s;
    bool lostPower, syncNTP;
    void (*notifyRTC)(void);
};

#endif