#include <Arduino.h>
#include "MicSensorTask.h"

void MicSensorTask::setPin(int PIN)
{
    _pin = PIN;
}

void MicSensorTask::setup()
{
    pinMode(_pin, INPUT);
}

void MicSensorTask::loop()
{
    if (_sound_detected == 1)
    {
        notifyClap();
        this->delay(5000);
        _sound_detected = 0;
    }
    else
    {
        int clap = digitalRead(_pin);
        if (clap == LOW)
        {
            _sound_detected = 1;
        }
        yield();
    }
}