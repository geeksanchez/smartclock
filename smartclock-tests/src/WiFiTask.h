#ifndef WiFiTask_h
#define WiFiTask_h

#include <Arduino.h>
#include <IotWebConf.h>
#include <Scheduler.h>

class WiFiTask : public Task
{
private:

protected:
    void setup();
    void loop();

public:
    IotWebConf *wifi;
};

#endif