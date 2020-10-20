#include <Arduino.h>
#include "RTClib.h"
#include "RTCTask.h"

RTC_DS3231 rtc;

void RTCTask::setup()
{
    lostPower = false;
    syncNTP = false;
    if (!rtc.begin())
    {
        _rtc_enabled = 0;
        Y = 2000;
        M = 1;
        D = 1;
        h = 0;
        m = 0;
        s = 0;
    }
    else
    {
        _rtc_enabled = 1;
        lostPower = rtc.lostPower();
        DateTime dt = rtc.now();
        Y = dt.year();
        M = dt.month();
        D = dt.day();
        h = dt.hour();
        m = dt.minute();
        s = dt.second();
        notifyRTC();
    }
    cycles = 0;
}

void RTCTask::loop()
{
    cycles++;
    if (_rtc_enabled == 1)
    {
        if (syncNTP)
        {
            rtc.adjust(DateTime(Y, M, D, h, m, s));
            syncNTP = false;
        }
        DateTime dt = rtc.now();
        Y = dt.year();
        M = dt.month();
        D = dt.day();
        h = dt.hour();
        m = dt.minute();
        s = dt.second();
        notifyRTC();
        cycles = 0;
        wait_time = 1000;
    }
    if (cycles > 10)
    {
        wait_time = 60000;
    }
    this->delay(wait_time);
}