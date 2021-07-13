#include <FastLED.h>        //https://github.com/FastLED/FastLED
#include <LEDMatrix.h>      //https://github.com/Jorgen-VikingGod/LEDMatrix

// Change the next defines to match your matrix type and size
#define DATA_PIN            4
#define CLOCK_PIN           5
#define DATA2_PIN           2
#define CLOCK2_PIN          0

#define COLOR_ORDER         BGR
#define CHIPSET             APA102

#define MATRIX_TILE_WIDTH   8 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT  32 // height of each matrix
#define MATRIX_TILE_H       3  // number of matrices arranged horizontally
#define MATRIX_TILE_V       1  // number of matrices arranged vertically
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL        (MATRIX_WIDTH*MATRIX_HEIGHT)

#define MATRIX_WIDTH        (MATRIX_TILE_WIDTH*MATRIX_TILE_H)
#define MATRIX_HEIGHT       (MATRIX_TILE_HEIGHT*MATRIX_TILE_V)

#define NUM_LEDS            (MATRIX_WIDTH*MATRIX_HEIGHT)

// create our matrix based on matrix definition
cLEDMatrix<-MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, HORIZONTAL_ZIGZAG_MATRIX, MATRIX_TILE_H, MATRIX_TILE_V, VERTICAL_BLOCKS> leds;

void matrix_clear() {
    //FastLED[1].clearLedData();
    // clear does not work properly with multiple matrices connected via parallel inputs
    memset(leds[0], 0, NUM_LEDS*3);
}

void count_pixels() {
    for (uint16_t i=0; i<MATRIX_HEIGHT; i++) {
	Serial.print(i, HEX);
	for (uint16_t j=0; j<MATRIX_WIDTH; j++) {
	    Serial.print(".");
	    leds.DrawPixel(j, i, i%3==0?CRGB::Blue:i%3==1?CRGB::Red:CRGB::Green);
	    // depending on the matrix size, it's too slow to display each pixel, so
	    // make the scan init faster. This will however be too fast on a small matrix.
	    #ifdef ESP8266
	    if (!(j%3)) FastLED.show();
	    yield(); // reset watchdog timer
	    #elif ESP32
	    delay(1);
	    FastLED.show();
	    #else 
	    FastLED.show();
	    #endif
	}
	Serial.println("");
    }
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  //FastLED.addLeds<CHIPSET, DATA_PIN,  CLOCK_PIN,  COLOR_ORDER>(leds[0], 0,             leds.Size()/2).setCorrection(TypicalSMD5050);
  //FastLED.addLeds<CHIPSET, DATA2_PIN, CLOCK2_PIN, COLOR_ORDER>(leds[0], leds.Size()/2, leds.Size()/2).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2811_PORTA,3>(leds[0], 256).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(32);
  FastLED.clear(true);
  matrix_clear();

#if 0
  delay(500);
  FastLED.showColor(CRGB::Red);
  delay(1000);
  FastLED.showColor(CRGB::Lime);
  delay(1000);
  FastLED.showColor(CRGB::Blue);
  delay(1000);
  FastLED.showColor(CRGB::White);
  delay(1000);
  FastLED.clear(true);
  matrix_clear();
#endif

  count_pixels();
  matrix_clear();
  leds.DrawLine (0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(0, 255, 0));
  leds.DrawPixel(0, 0, CRGB(255, 0, 0));
  leds.DrawPixel(leds.Width() - 1, leds.Height() - 1, CRGB(0, 0, 255));
  FastLED.show();
  Serial.println("Colors done");
  delay(10000);

  // Scottish Flag
  leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(0, 0, 255));
  leds.DrawRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawLine(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawLine(0, 1, leds.Width() - 1, leds.Height() - 2, CRGB(255, 255, 255));
  leds.DrawLine(0, leds.Height() - 1, leds.Width() - 1, 0, CRGB(255, 255, 255));
  leds.DrawLine(0, leds.Height() - 2, leds.Width() - 1, 1, CRGB(255, 255, 255));
  FastLED.show();
  delay(5000);
  Serial.println("Flag #1 done");

  // Japanese Flag
  leds.DrawFilledRectangle(0, 0, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  uint16_t r = min((leds.Width() - 1) / 2, (leds.Height() - 1) / 2) - 1;
  leds.DrawFilledCircle((leds.Width() - 1) / 2, (leds.Height() - 1) / 2, r, CRGB(255, 0, 0));
  FastLED.show();
  delay(5000);
  Serial.println("Flag #2 done");

  // Norwegian Flag
  int16_t x = (leds.Width() / 4);
  int16_t y = (leds.Height() / 2) - 2;
  leds.DrawFilledRectangle(0, 0, x, y, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(0, 0, x - 1, y - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(x + 3, 0, leds.Width() - 1, y, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(x + 4, 0, leds.Width() - 1, y - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(0, y + 3, x, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(0, y + 4, x - 1, leds.Height() - 1, CRGB(255, 0, 0));
  leds.DrawFilledRectangle(x + 3, y + 3, leds.Width() - 1, leds.Height() - 1, CRGB(255, 255, 255));
  leds.DrawFilledRectangle(x + 4, y + 4, leds.Width() - 1, leds.Height() - 1, CRGB(255, 0, 0));
  leds.DrawLine(0, y + 1, leds.Width() - 1, y + 1, CRGB(0, 0, 255));
  leds.DrawLine(0, y + 2, leds.Width() - 1, y + 2, CRGB(0, 0, 255));
  leds.DrawLine(x + 1, 0, x + 1, leds.Height() - 1, CRGB(0, 0, 255));
  leds.DrawLine(x + 2, 0, x + 2, leds.Height() - 1, CRGB(0, 0, 255));
  FastLED.show();
  delay(5000);
  leds.ShiftLeft();
  delay(5000);
  Serial.println("Flag #3 done");
}


void loop()
{
#if 0
  uint8_t h = sin8(angle);
  leds.ShiftLeft();
  for (int16_t y=leds.Height()-1; y>=0; --y)
  {
    yield();
    leds(leds.Width()-1, y) = CHSV(h, 255, 255);
    h += 32;
  }
  angle += 4;
  FastLED.show();
#endif
  delay(20);
}
