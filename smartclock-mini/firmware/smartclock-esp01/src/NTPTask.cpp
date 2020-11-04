#include "NTPTask.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", (-5 * 60 * 60));

unsigned long lastMillis;

void NTPTask::setup()
{
    timeClient.begin();
    lastMillis = millis();
    epochTime = 0;
}

void NTPTask::loop()
{
    if ((millis() - lastMillis) > 20000)
    {
        if (timeClient.update())
        {
            if ((timeClient.getEpochTime() - epochTime) >= 60)
            {
                epochTime = timeClient.getEpochTime();
                notify();
            }
        }
        else
        {
            Serial.println("NTP update: fail");
        }
        lastMillis = millis();
    }
    yield();
}