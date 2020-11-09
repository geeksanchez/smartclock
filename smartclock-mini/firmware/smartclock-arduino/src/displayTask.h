#ifndef displayTask_h
#define displayTask_h

#include "Task.h"
#include "TaskScheduler.h"

class displayTask : public Task
{
private:

protected:
    displayTask();
    virtual void run(uint32_t now);
    virtual bool canRun(uint32_t now);

public:
};

#endif