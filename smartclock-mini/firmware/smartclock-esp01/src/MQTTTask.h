#ifndef MQTTTask_h
#define MQTTTask_h

#include <Arduino.h>
#include <Scheduler.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE (256)

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
    void (*notifyMQTT)();
    void publish(const char *topic, const char *payload);
};

#endif