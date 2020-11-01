#ifndef NTPTask_h
#define NTPTask_h

#include <Scheduler.h>

class NTPTask : public Task
{

private:
    int wait_time;

protected:
    void setup();
    void loop();

public:
    void (*notify)(void);
    unsigned long epochTime;
};

#endif