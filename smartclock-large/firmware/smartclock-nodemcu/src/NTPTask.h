#ifndef NTPTask_h
#define NTPTask_h

#include <Scheduler.h>

class NTPTask : public Task
{

private:
    int wait_time;
    int cycles;

protected:
    void setup();
    void loop();

public:
    int Y, M, D, h, m, s;
    void (*notifyNTP)(void);
};

#endif