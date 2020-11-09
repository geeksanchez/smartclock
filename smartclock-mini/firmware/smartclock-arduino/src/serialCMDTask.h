#ifndef serialCMDTask_h
#define serialCMDTask_h

#include "Task.h"
#include "TaskScheduler.h"

class serialCMDTask : public Task
{
private:
    uint16_t byteCount = 0;
    char msg[256], last_msg[256];
    bool new_message;

protected:
    serialCMDTask();
    virtual void run(uint32_t now);
    virtual bool canRun(uint32_t now);

public:
    void espWrite(String espMsg);
    String espRead();

};

#endif