#ifndef OTATask_h
#define OTATask_h

#include <Arduino.h>
#include <Scheduler.h>

class OTATask : public Task
{

private:

protected:
    void setup();
    void loop();

public:
    char msg[20];
    void (*notifyOTA)(void);
};

#endif