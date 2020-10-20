#ifndef MicSensorTask_h
#define MicSensorTask_h

#include <Arduino.h>
#include <Scheduler.h>

class MicSensorTask : public Task
{

    private:
        uint8_t _sound_detected;
        int _pin = D4;

    protected :
        void setup();
        void loop();

    public:
        void setPin(int PIN);
        void (*notifyClap)(void);
};

#endif