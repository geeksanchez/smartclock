#include <Arduino.h>
#include "MQTTTask.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *mqtt_server = "broker.hivemq.com";
char mqttMsg[128];

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void MQTTTask::publish(const char *topic, const char *payload)
{
    if (mqttClient.connected())
    {
        mqttClient.publish(topic, payload);
    }
}

void MQTTTask::reconnect()
{
    while (!mqttClient.connected())
    {
        // Create a random client ID
        String clientId = "smartclock-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (mqttClient.connect(clientId.c_str()))
        {
            char ssid[50];
            snprintf(ssid, 50, "sansila/smartclock/smartclock-%X/salida", ESP.getChipId());
            // Once connected, publish an announcement...
            mqttClient.publish(ssid, "smartclock connected!");
            snprintf(ssid, 50, "sansila/smartclock/smartclock-%X/entrada", ESP.getChipId());
            // ... and resubscribe
            mqttClient.subscribe(ssid);
        }
        else
        {
            // Wait 5 seconds before retrying
            this->delay(5000);
        }
    }
}

void MQTTTask::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    char *p = (char *)malloc(length);
    memcpy(p, payload, length);
    p[length] = '\0';
    strcpy(command, p);
    notifyMQTT();
    free(p);
}

void MQTTTask::setup()
{
    strcpy(command, "INV");
    std::function<void(char *, uint8_t *, unsigned int)> callback = [=](char *topic, byte *payload, unsigned int length) {
        this->mqttCallback(topic, payload, length);
    };
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callback);
}

void MQTTTask::loop()
{
    if (!mqttClient.connected())
    {
        reconnect();
    }
    mqttClient.loop();
    yield();
}