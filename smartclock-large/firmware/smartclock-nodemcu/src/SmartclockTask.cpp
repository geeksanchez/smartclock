#include "SmartclockTask.h"

void SmartclockTask::setup()
{
    notifySmartclock(1);
}

void SmartclockTask::loop()
{
    yield();
}