#include "Icons.h"
#include "Font3x5.h"
#include <FastLED.h>
#include <MatrixTask.h>

#define DATA_PIN 3
#define NUM_LEDS 256
#define BRIGHTNESS 5

CRGBArray<NUM_LEDS> leds;

void MatrixTask::setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    notifyMatrix(NULL);
}

void MatrixTask::Cls()
{
    leds.fill_solid(CRGB::Black);
    FastLED.show();
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
/*    char c = letter;
    // Space is often not included in font so use width of 'n'
    if (c == ' ')
        c = 'n';
    uint8_t width = 0;

    uint8_t firstChar = pgm_read_byte(this->Font + FONT_FIRST_CHAR);
    uint8_t charCount = pgm_read_byte(this->Font + FONT_CHAR_COUNT);

    uint16_t index = 0;

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
    return width;*/
}

void MatrixTask::drawString(int bX, int bY, const char *bChars, byte length,
                     byte fgcolour, byte bgcolour)
{
/*    if (bX >= DisplayMaxX || bY >= DisplayMaxY)
        return;
    uint8_t height = pgm_read_byte(this->Font + FONT_HEIGHT);
    if (bY + height < 0)
        return;

    int strWidth = 0;
    this->drawLine(bX - 1, bY, bX - 1, bY + height, bgcolour);

    for (byte i = 0; i < length; i++)
    {
        int charWide = this->drawChar(bX + strWidth, bY, bChars[i], fgcolour, bgcolour);
        if (charWide > 0)
        {
            strWidth += charWide;
            this->drawLine(bX + strWidth, bY, bX + strWidth, bY + height, bgcolour);
            strWidth++;
        }
        else if (charWide < 0)
        {
            return;
        }
        if ((bX + strWidth) >= DisplayMaxX || bY >= DisplayMaxY)
            return;
    }*/
}

void MatrixTask::DrawMsg(char *msg, byte length)
{
/*    marqueeWidth = 0;
    for (byte i = 0; i < length; i++)
    {
        MtxMsg[i] = msg[i];
        marqueeWidth += charWidth(msg[i]) + 1;
    }
    marqueeHeight = pgm_read_byte(this->Font + FONT_HEIGHT);
    marqueeText[length] = '\0';
    marqueeOffsetY = top;
    marqueeOffsetX = left;
    marqueeLength = length;
    marqueeColour = fgcolour;
    marqueeBG = bgcolour;
    drawString(marqueeOffsetX, marqueeOffsetY, marqueeText, marqueeLength,
               marqueeColour, marqueeBG);*/
}

void MatrixTask::DrawTime(char *msg)
{
    uint8_t pos_inicial, pos, val;
    uint8_t letra;
    for (int i = 0; i < 192; i++)
    {
        leds[i+64] = CRGB::Black;
    }
    leds[82 + 64] = CRGB(128, 128, 128);
    leds[106+64] = CRGB(128, 128, 128);
    for (uint8_t j = 0; j < 4; j++)
    {
        if (j == 0)
        {
            pos_inicial = 10 + 64;
        } else if (j == 1)
        {
            pos_inicial = 14 + 64;
        } else if (j == 2)
        {
            pos_inicial = 76 + 64;
        } else
        {
            pos_inicial = 136 + 64;
        }
//        notifyMatrix("\n");
        for (uint8_t i = 0; i < 5; i++)
        {
            letra = pgm_read_byte(font3x5 + ((msg[j] - '0' + 16) * 5) + i);
//            itoa(letra, text, 10);
//            notifyMatrix(text);
            val = (letra & 0x04) >> 2;
            if (val == 1)
            {
                pos = pos_inicial + (i * 8);
                leds[pos] = CRGB(128, 128, 128);
            }
            val = (letra & 0x02) >> 1;
            if (val == 1)
            {
                pos = pos_inicial + 1 + (i * 8);
                leds[pos] = CRGB(128, 128, 128);
            }
            val = (letra & 0x01);
            if (val == 1)
            {
                if (j == 1)
                {
                    pos = 72 + 64 + (i * 8);
                }
                else
                {
                    pos = pos_inicial + 2 + (i * 8);
                }
                leds[pos] = CRGB(128, 128,128);
            }
        }
    }
    FastLED.show();
}

void MatrixTask::DrawDate(char *msg)
{
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
