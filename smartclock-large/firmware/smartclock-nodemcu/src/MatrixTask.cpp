#include "constants.h"
#include "Icons.h"
#include "Font3x5.h"
#include <FastLED.h>
#include <MatrixTask.h>

#define DATA_PIN 3
#define NUM_LEDS 256
#define BRIGHTNESS 5
#define DisplayMaxX 24
#define DisplayMaxY 8

CRGBArray<NUM_LEDS> leds;

void MatrixTask::setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    selectFont(Font3x5);
    notifyMatrix(NULL);
}

void MatrixTask::clearText()
{
    fill_solid(leds+64, 192, CRGB::Black);
    FastLED.show();
}

void MatrixTask::writePixel(unsigned int bX, unsigned int bY,  uint32_t colour)
{
    if (bX >= DisplayMaxX || bY >= DisplayMaxY)
    {
        return;
    }
    uint8_t pos = ((bX / 8) * 64) + (bX % 8) + (bY * 8) + 64;
    leds[pos] = colour;
}

void MatrixTask::DrawIcon(uint8_t icon)
{
    uint32_t bitmap[64];
    memcpy_P(bitmap, dib[icon], 64*4);
    for(uint8_t j=0; j<8; j++)
    {
        for(uint8_t i=0; i<8; i++)
        {
            leds[i + j * 8] = CRGB((bitmap[i + j * 8]) & 0xFF, (bitmap[i + j * 8] >> 8) & 0xFF, (bitmap[i + j * 8] >> 16) & 0xFF);
        }
    }
    FastLED.show();
}

void MatrixTask::selectFont(const uint8_t *font)
{
    this->Font = font;
}

byte MatrixTask::charWidth(const char letter)
{
    char c = letter;
    // Space is often not included in font so use width of 'n'
    if (c == ' ')
        c = 'n';
    uint8_t width = 0;

    uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
    uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);

    if (c < firstChar || c >= (firstChar + charCount))
    {
        return 0;
    }
    c -= firstChar;

    if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0)
    {
        // zero length is flag indicating fixed width font (array does not contain width data entries)
        width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
    }
    else
    {
        // variable width font, read width data
        width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
    }
    return width;
}

byte MatrixTask::drawChar(const int bX, const int bY, const char letter, uint32_t fgcolour, uint32_t bgcolour)
{
    if (bX >= DisplayMaxX || bY >= DisplayMaxY)
        return -1;
    char c = letter;
    uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);
    if (c == ' ')
    {
        byte charWide = charWidth(' ');
//        this->drawFilledBox(bX, bY, bX + charWide, bY + height, bgcolour);
        return charWide;
    }
    uint8_t width = 0;
    uint8_t bytes = (height + 7) / 8;

    uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
    uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);

    uint16_t index = 0;

    if (c < firstChar || c >= (firstChar + charCount))
        return 0;
    c -= firstChar;

    if (pgm_read_byte(this->Font + FONT_LENGTH) == 0 && pgm_read_byte(this->Font + FONT_LENGTH + 1) == 0)
    {
        // zero length is flag indicating fixed width font (array does not contain width data entries)
        width = pgm_read_byte(this->Font + FONT_FIXED_WIDTH);
        index = c * bytes * width + FONT_WIDTH_TABLE;
    }
    else
    {
        // variable width font, read width data, to get the index
        for (uint8_t i = 0; i < c; i++)
        {
            index += pgm_read_byte(this->Font + FONT_WIDTH_TABLE + i);
        }
        index = index * bytes + charCount + FONT_WIDTH_TABLE;
        width = pgm_read_byte(this->Font + FONT_WIDTH_TABLE + c);
    }
    if (bX < -width || bY < -height)
        return width;

    // last but not least, draw the character
    for (uint8_t j = 0; j < width; j++)
    { // Width
        for (uint8_t i = bytes - 1; i < 254; i--)
        { // Vertical Bytes
            uint8_t data = pgm_read_byte(this->Font + index + j + (i * width));
            int offset = (i * 8);
            if ((i == bytes - 1) && bytes > 1)
            {
                offset = height - 8;
            }
            else if (height < 8)
            {
                offset = height - 7;
            }
            for (uint8_t k = 0; k < 8; k++)
            { // Vertical bits
                if ((offset + k >= i * 8) && (offset + k <= height))
                {
                    if (data & (1 << k))
                    {
                        writePixel(bX + j, bY + offset + k, fgcolour);
                    }
                    else
                    {
                        writePixel(bX + j, bY + offset + k, bgcolour);
                    }
                }
            }
        }
    }
    return width;
}

void MatrixTask::drawString(int bX, int bY, const char *bChars, byte length,
                     uint32_t fgcolour, uint32_t bgcolour)
{
    if (bX >= DisplayMaxX || bY >= DisplayMaxY)
        return;
    uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);
    if (bY + height < 0)
        return;

    int strWidth = 0;
//    this->drawLine(bX - 1, bY, bX - 1, bY + height, bgcolour);

    for (byte i = 0; i < length; i++)
    {
        int charWide = this->drawChar(bX + strWidth, bY, bChars[i], fgcolour, bgcolour);
        if (charWide > 0)
        {
            strWidth += charWide;
//            this->drawLine(bX + strWidth, bY, bX + strWidth, bY + height, bgcolour);
            strWidth++;
        }
        else if (charWide < 0)
        {
            return;
        }
        if ((bX + strWidth) >= DisplayMaxX || bY >= DisplayMaxY)
            return;
    }
}

void MatrixTask::ScrollMsg(char *msg, byte length)
{
}

void MatrixTask::DrawMsg(char *msg, byte length, uint8_t mode)
{
    switch (mode)
    {
    case TIME_MODE:
        if (length == 4)
        {
            clearText();
            drawString(4, 0, msg, 2, CRGB::White, CRGB::Black);
            drawString(12, 0, ":", 2, CRGB::White, CRGB::Black);
            drawString(14, 0, msg + 2, 2, CRGB::White, CRGB::Black);
            FastLED.show();
        }
        break;
    case DATE_MODE:
        if (length == 6)
        {
            drawString(2, 0, msg, 2, CRGB::White, CRGB::Black);
            drawString(10, 0, "/", 2, CRGB::White, CRGB::Black);
            drawString(12, 0, msg + 2, 2, CRGB::White, CRGB::Black);
            FastLED.show();
        }
        break;
    case TEXT_MODE:
        drawString(2, 0, msg, length, CRGB::White, CRGB::Black);
        FastLED.show();
        break;
    case SCROLL_MODE:
        break;
    default:
        break;
    }
}

void MatrixTask::setBrightness(uint8_t lux)
{
    FastLED.setBrightness(lux);
    FastLED.show();
}

void MatrixTask::loop()
{
    yield();
}
