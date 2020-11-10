#ifndef serialCMDTask_h
#define serialCMDTask_h

#include "Task.h"
#include "TaskScheduler.h"
#include <SoftwareSerial.h>
#include "Debugger.h"

SoftwareSerial espSerial(2, 3);

class serialCMDTask : public Task
{
private:
    uint16_t byteCount = 0;
    char msg[256], last_msg[256];
    bool new_message;
    Debugger *ptrDebugger;

protected:
public:
    serialCMDTask(Debugger *_ptrDebugger = NULL);
    virtual void run(uint32_t now);
    virtual bool canRun(uint32_t now);
    void espWrite(String espMsg);
    String espRead();
    void (*notify)(void);
};

serialCMDTask::serialCMDTask(Debugger *_ptrDebugger) : Task(),
                                                       ptrDebugger(_ptrDebugger)
{
    espSerial.begin(9600);
#ifdef DEBUG
    ptrDebugger->debugWrite("ESP serial communication channel starting");
#endif
}

bool serialCMDTask::canRun(uint32_t now)
{
    return (espSerial.available() > 0);
}

void serialCMDTask::run(uint32_t now)
{
    while (espSerial.available() > 0)
    {
        char ch = espSerial.read();
        if (byteCount > 255)
        {
            byteCount = 0;
            msg[byteCount] = '\0';
#ifdef DEBUG
            ptrDebugger->debugWrite("-------------------");
            ptrDebugger->debugWrite("Buffer overflow");
            ptrDebugger->debugWrite("-------------------");
#endif
        }
        else
        {
            if ((ch != '\n') && (ch != '\r'))
            {
                msg[byteCount] = ch;
                byteCount++;
            }
            else
            {
                if (ch == '\n')
                {
                    msg[byteCount] = '\0';
                    new_message = true;
                    strcpy(last_msg, msg);
                    byteCount = 0;
                    notify();
#ifdef DEBUG
                    ptrDebugger->debugWrite("-------------------");
                    ptrDebugger->debugWrite(last_msg);
                    ptrDebugger->debugWrite("-------------------");
#endif
                }
            }
        }
    }
}

String serialCMDTask::espRead()
{
    return String(last_msg);
}

#endif