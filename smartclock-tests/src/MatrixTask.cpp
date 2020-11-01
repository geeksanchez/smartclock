#include "MatrixTask.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Fonts/TomThumb.h>

void MatrixTask::setup()
{
    ready = 0;
    matrix = new FastLED_NeoMatrix(leds, 8, 8, 4, 1, 
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE +
        NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_PROGRESSIVE);

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(10);
    matrix->setFont(&TomThumb);
    matrix->fillScreen(0);
    matrix->show();
    ready = 1;
}

void MatrixTask::loop()
{
    yield();
}

void MatrixTask::showBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h)
{
    uint32_t colour;
    for (uint8_t i = 0; i < h; i++)
    {
        for (uint8_t j = 0; j < w; j++)
        {
            colour = (uint32_t)((bitmap[(i * w + j) * 3] << 0) + (bitmap[(i * w + j) * 3 + 1] << 8) + (bitmap[(i * w + j) * 3 + 2] << 16));
            matrix->drawPixel(x-w+j+1, y-i, colour);
        }
    }
    matrix->show();
}

void MatrixTask::setPixel(uint16_t x, int16_t y, uint32_t colour)
{
    matrix->drawPixel(x, y, colour);
}

void MatrixTask::setText(uint16_t x, int16_t y, String msg, uint32_t colour) 
{
    matrix->setTextColor(colour);
    matrix->setCursor(x, y);
    matrix->print(msg);
    matrix->show();
}

void MatrixTask::lux(uint8_t i)
{
    matrix->setBrightness(i);
}

void MatrixTask::clear()
{
    matrix->fillScreen(0);
    matrix->show();
}
