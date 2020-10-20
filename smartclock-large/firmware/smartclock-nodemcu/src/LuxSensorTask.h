#ifndef LuxSensorTask_h
#define LuxSensorTask_h

#include <Arduino.h>
#include <Scheduler.h>

class LuxSensorTask : public Task 
{

    private:
        int _lux_enabled;
        int wait_time;
        int cycles;

    protected:
        void setup();
        void loop();

    public:
        uint8_t lux;
        void (*notifyLux) (void);

};

#endif