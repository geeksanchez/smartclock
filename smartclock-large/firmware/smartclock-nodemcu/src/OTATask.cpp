#include <Arduino.h>
#include "OTATask.h"
#include <ArduinoOTA.h>

void OTATask::setup()
{
    char hostname[20];
    sprintf(hostname, "smartclock-%X", ESP.getChipId());
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(hostname);

    ArduinoOTA.onStart([=]() {
        strcpy(this->msg, "OTA Start");
        yield();
        notifyOTA(this->msg);
    });
    ArduinoOTA.onEnd([=]() {
        strcpy(this->msg, "OTA End");
        yield();
        notifyOTA(this->msg);
    });
    ArduinoOTA.onProgress([=](unsigned int progress, unsigned int total) {
        sprintf(this->msg, "Progress: %u%%", (progress / (total / 100)));
        yield();
    });
    ArduinoOTA.onError([=](ota_error_t error) {
        if (error == OTA_AUTH_ERROR)
            sprintf(this->msg, "Error[%u]: Auth Failed", error);
        else if (error == OTA_BEGIN_ERROR)
            sprintf(this->msg, "Error[%u]: Begin Failed", error);
        else if (error == OTA_CONNECT_ERROR)
            sprintf(this->msg, "Error[%u]: Connect Failed", error);
        else if (error == OTA_RECEIVE_ERROR)
            sprintf(this->msg, "Error[%u]: Receive Failed", error);
        else if (error == OTA_END_ERROR)
            sprintf(this->msg, "Error[%u]: End Failed", error);
        yield();
        notifyOTA(this->msg);
    });
    ArduinoOTA.begin();
}

void OTATask::loop()
{
    ArduinoOTA.handle();
    yield();
}