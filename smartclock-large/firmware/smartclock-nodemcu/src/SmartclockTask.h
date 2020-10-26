#ifndef SmartclockTask_h
#define SmartclockTask_h

#include <Arduino.h>
#include <Scheduler.h>

class SmartclockTask : public Task
{

private:

protected:
    void setup();
    void loop();

public:
    void (*notifySmartclock)(uint8_t state, char *msg, int len);

};

#endif