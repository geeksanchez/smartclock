#include "SmartclockTask.h"

void SmartclockTask::setup()
{
    st = 1;
}

void SmartclockTask::start()
{
    st = 1;
}

    void SmartclockTask::loop()
{
    if (st == 1)
    {
        notifySmartclock(1);
        st = 0;
    }
    yield();
}