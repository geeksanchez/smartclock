#ifndef OTATask_h
#define OTATask_h

#include <Arduino.h>
#include <Scheduler.h>

class OTATask : public Task
{

private:
    char msg[20];

protected:
    void setup();
    void loop();

public:

};

#endif