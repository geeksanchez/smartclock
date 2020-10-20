#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <BH1750.h>
#include "LuxSensorTask.h"

BH1750 lightMeter;

void LuxSensorTask::setup()
{
    _lux_enabled = 0;
    Wire.begin();
    if (lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE))
    {
        _lux_enabled = 1;
        Serial.println(F("Lux sensor enabled."));
    }
    else
    {
        _lux_enabled = 0;
        Serial.println(F("Lux sensor not found."));
    }
    cycles = 0;
    wait_time = 10000;
}

void LuxSensorTask::loop()
{
    if (_lux_enabled == 1)
    {
        float luxValue;
        luxValue = lightMeter.readLightLevel(true);
        yield();
        notifyLux();
        if (luxValue < 0)
        {
            lux = 0;
            Serial.println(F("Error condition detected"));
        }
        else
        {
            lux = luxValue;
            if (luxValue > 40000.0)
            {
                // reduce measurement time - needed in direct sun light
                if (!lightMeter.setMTreg(32))
                {
                    Serial.println(F("Error setting MTReg to low value for high light environment"));
                }
            }
            else
            {
                if (luxValue > 10.0)
                {
                    // typical light environment
                    if (!lightMeter.setMTreg(69))
                    {
                        Serial.println(F("Error setting MTReg to default value for normal light environment"));
                    }
                }
                else
                {
                    if (luxValue <= 10.0)
                    {
                        //very low light environment
                        if (!lightMeter.setMTreg(138))
                        {
                            Serial.println(F("Error setting MTReg to high value for low light environment"));
                        }
                    }
                }
            }
        }
        this->delay(wait_time);
    }
    yield();
}
