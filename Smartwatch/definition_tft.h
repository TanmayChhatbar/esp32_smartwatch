/*
  Definitions related to the TFT
*/

#define BUTTON1PIN 35                         //Top
#define BUTTON2PIN 0                          //Bottom
#define TFT_BL 4                              //TFT Backlight
#define tftheight 135
#define tftwidth 240
#define displaytimeout (3 * 1000)             //seconds in millis
#define inactivitytimeout (5 * 60 * 1000)     //minutes in millis
TFT_eSPI tft = TFT_eSPI(tftheight, tftwidth);
