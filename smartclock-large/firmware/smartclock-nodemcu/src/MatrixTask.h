#ifndef MatrixTask_h
#define MatrixTask_h

#include <Arduino.h>
#include <Scheduler.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define DATA_PIN 3
#define NUM_LEDS 256
#define DISPLAY_MAX_X 32
#define DISPLAY_MAX_Y 8

class MatrixTask : public Task
{
private:
    CRGB leds[256];
    FastLED_NeoMatrix *matrix;

protected:
    void setup();
    void loop();
    
public:
    uint8_t ready;
    void showBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h);
    void setPixel(int16_t x, int16_t y, uint32_t colour);
    void setText(int16_t x, int16_t y, String msg, uint32_t colour);
    void lux(uint8_t i);
    void clear();
};

#endif