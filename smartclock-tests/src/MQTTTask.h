#ifndef MQTTTask_h
#define MQTTTask_h

#include <Arduino.h>
#include <Scheduler.h>

#ifndef MSG_BUFFER_SIZE
#define MSG_BUFFER_SIZE (256)
#endif

class MQTTTask : public Task
{

private:
    void reconnect();
    void mqttCallback(char *topic, byte *payload, unsigned int length);

protected : 
    void setup();
    void loop();

public:
    char command[MSG_BUFFER_SIZE];
    const char *title;
    void (*notify)();
    void publish(const char *topic, const char *payload);
};

#endif