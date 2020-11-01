#include "NTPTask.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "horalegal.inm.gov.co", (-5 * 60 * 60));

unsigned long lastMillis;

void NTPTask::setup()
{
    timeClient.begin();
    lastMillis = millis();
}

void NTPTask::loop()
{
    if ((millis() - lastMillis) > 60000)
    {
        timeClient.update();
        if ((timeClient.getEpochTime() - epochTime) >= 60)
        {
            epochTime = timeClient.getEpochTime();
            notify();
        }
        lastMillis = millis();
    }
    yield();
}