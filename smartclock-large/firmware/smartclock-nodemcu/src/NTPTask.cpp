#include "NTPTask.h"
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "horalegal.inm.gov.co", (-5 * 60 * 60));

void NTPTask::setup()
{
    timeClient.begin();
    Y = 2000;
    M = 1;
    D = 1;
    h = 0;
    m = 0;
    s = 0;
    cycles = 0;
    wait_time = 1000;
}

void NTPTask::loop()
{
    cycles++;
    if (timeClient.update())
    {
        unsigned long epochTime = timeClient.getEpochTime();
        struct tm *ptm = gmtime((time_t *)&epochTime);

        Y = ptm->tm_year + 1900;
        M = ptm->tm_mon + 1;
        D = ptm->tm_mday;
        h = ptm->tm_hour;
        m = ptm->tm_min;
        s = ptm->tm_sec;
        notifyNTP();
        cycles = 0;
        wait_time = 1000;
    }
    if (cycles > 10)
    {
        wait_time = 60000;
    }
    this->delay(wait_time);
}