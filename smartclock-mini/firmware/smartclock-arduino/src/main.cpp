// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution
// of 176X220 pixels.
//
// This program requires the UTFT library.

//Demo for
//by Open-Smart Team and Catalex Team
//catalex_inc@163.com
//Store: open-smart.aliexpress.com
//      http://dx.com
//Demo Function:

#include "TimeLib.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "serialCMDTask.h"
#include "main.h"


char msg[256], last_msg[256];
uint8_t count = 0;
uint8_t last_minute = 0;
uint8_t state = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Smartclock starting...");
  delay(3000);
  // Setup ESP-01 serial
  espSerial.begin(9600);
  count = 0;
  last_msg[0] = '\0';
  msg[0] = '\0';
  tft.clrScr();
  state = 1;
}

void drawWallpaper(char *text)
{
  tft.setColor(VGA_BLACK);
  tft.fillRect(0, 200, 175, 219);
  tft.setFont(SmallFont);
  tft.setColor(VGA_GREEN);
  tft.print(text, CENTER, 200);
}

void drawTime(unsigned long t)
{
  char h[] = "123";
  char m[] = "000";
  uint8_t posY;
  uint8_t posX;
  uint8_t dX = minute();
  if (last_minute != minute())
  {
    tft.setColor(VGA_BLACK);
    tft.fillCircle(88, 55, 50);
    tft.setColor(VGA_WHITE);
    tft.drawCircle(88, 55, 50);

    posY = pgm_read_byte(minutosY + dX);
    posX = pgm_read_byte(minutosX + dX);
    tft.setColor(VGA_BLUE);
    if (minute() < 16)
    {
      tft.drawLine(88, 55, 88 + (((50 * posX) >> 8) & 0xFF), 55 - (((50 * posY) >> 8) & 0xFF));
    }
    else if (minute() < 31)
    {
      tft.drawLine(88, 55, 88 + (((50 * posX) >> 8) & 0xFF), 55 + (((50 * posY) >> 8) & 0xFF));
    }
    else if (minute() < 46)
    {
      tft.drawLine(88, 55, 88 - (((50 * posX) >> 8) & 0xFF), 55 + (((50 * posY) >> 8) & 0xFF));
    }
    else
    {
      tft.drawLine(88, 55, 88 - (((50 * posX) >> 8) & 0xFF), 55 - (((50 * posY) >> 8) & 0xFF));
    }
    last_minute = minute();
    dX = hour();
    posY = pgm_read_byte(horasY + dX);
    posX = pgm_read_byte(horasX + dX);
    tft.setColor(VGA_RED);
    if (hourFormat12() < 4)
    {
      tft.drawLine(88, 55, 88 + (((30 * posX) >> 8) & 0xFF), 55 - (((30 * posY) >> 8) & 0xFF));
    }
    else if (hourFormat12() < 7)
    {
      tft.drawLine(88, 55, 88 + (((30 * posX) >> 8) & 0xFF), 55 + (((30 * posY) >> 8) & 0xFF));
    }
    else if (hourFormat12() < 10)
    {
      tft.drawLine(88, 55, 88 - (((30 * posX) >> 8) & 0xFF), 55 + (((30 * posY) >> 8) & 0xFF));
    }
    else
    {
      tft.drawLine(88, 55, 88 - (((30 * posX) >> 8) & 0xFF), 55 - (((30 * posY) >> 8) & 0xFF));
    }

    tft.setFont(SevenSegNumFont);
    tft.setColor(VGA_WHITE);
    snprintf(h, 3, "%d", hour());
    if (minute() < 10)
    {
      snprintf(m, 3, "0%d", minute());
    }
    else
    {
      snprintf(m, 3, "%d", minute());
    }
    if (hour() < 10)
    {
      tft.print(h, 50, 120);
    }
    else
    {
      tft.print(h, 12, 120);
    }
    tft.print(m, 100, 120);
    tft.fillRoundRect(85, 130, 94, 139);
    tft.fillRoundRect(85, 150, 94, 159);
  }
}

void processCMD(char *texto)
{
  char cmd[256];
  char *rest = texto;
  char *token = strtok_r(rest, " ", &rest);
  if (token != NULL)
  {
    if (strcmp(token, "NTP") == 0)
    {
      setTime(atol(rest));
      if (last_minute != minute())
      {
        drawTime(now());
        state = 3;
      }
    }
    else if (strcmp(token, "UPTIME") == 0)
    {
      snprintf(cmd, 256, "UPTIME: %lums\n", millis());
      espSerial.print(cmd);
      state = 2;
    }
    else if (strcmp(token, "AMSG") == 0)
    {
      drawWallpaper(rest);
      state = 0;
    }
  }
}

void loop()
{

  switch (state)
  {
    case 0:
      break;
    case 1:
      drawWallpaper("Waiting WiFi...");
      espSerial.print("Starting smartclock arduino...\n");
      state = 0;
      break;
    case 2:
      drawWallpaper("Mensaje desde internet");
      state = 0;
      break;
    case 3:
      drawWallpaper("Hora en Colombia");
      state = 0;
      break;
    default:
      break;
  }
  state = 0;    
  
}
