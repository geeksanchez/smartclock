#include "SmartclockTask.h"

#define PERIOD 500

unsigned long curtime;
unsigned long prevtime;
uint8_t start;

void SmartclockTask::setup()
{
    prevtime = millis();
    start = 1;
}

void SmartclockTask::loop()
{
    curtime = millis();
    if ((curtime - prevtime) >= PERIOD)
    {
        prevtime = curtime;
        if (start == 1) 
        {
            notifySmartclock(1, NULL, 0);
            start = 0;
        }
        else
        {
            notifySmartclock(0, NULL, 0);
        }
    }
    yield();
}