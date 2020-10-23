#ifndef SmartclockTask_h
#define SmartclockTask_h

#include <Arduino.h>
#include <Scheduler.h>

class SmartclockTask : public Task
{

private:
    uint8_t st;

protected:
    void setup();
    void loop();

public:
    void (*notifySmartclock)(uint8_t state);
    void start();
};

#endif