#include <UTFT.h>
#include "displayTask.h"

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];

//***********************************************//
// If you use OPEN-SMART TFT breakout board                 //
// You need to add 5V-3.3V level converting circuit.
// Of course you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V.
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- OPEN-SMART UNO Black /Red
// GND              -- GND
// 3V3               -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                -- A1
// RD                 -- 3.3V
// RST                -- A0
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7

//
// Remember to change the model parameter to suit your display module!
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RS A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RST 5 //

UTFT tft(ILI9225, LCD_RS, LCD_WR, LCD_CS, LCD_RST);

displayTask::displayTask() : Task()
{
    tft.InitLCD(PORTRAIT);
    tft.setFont(SmallFont);
    tft.setColor(VGA_WHITE);
    tft.print("Starting smartclock", CENTER, 50);
    tft.print("please wait...", CENTER, 100);
}

bool displayTask::canRun(uint32_t now)
{

}

void displayTask::run(uint32_t now)
{

}