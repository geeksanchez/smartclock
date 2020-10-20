#ifndef CMDTask_h
#define CMDTask_h

#include <Arduino.h>
#include <Scheduler.h>

class CMDTask : public Task
{
private:

protected:
    void setup();
    void loop();

public:
    void sendMSG(char *msg);
    void (*recvMSG)(char *msg);
};

#endif