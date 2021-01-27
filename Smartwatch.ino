/*
 * features to add
 *  music control
*/

//FreeRTOS reference https://www.youtube.com/watch?v=NEq-L9TNMts
//Libraries
#include <TFT_eSPI.h>     //https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_eSPI.h
#include <SPI.h>          //for TFT
#include <WiFi.h>         //for time
#include "time.h"         //for time
#include <Wire.h>         //for accelerometer

//TFT related
#define BUTTON1PIN 35 //Top
#define BUTTON2PIN 0  //Bottom
#define TFT_BL 4
#define tftheight 135
#define tftwidth 240
#define displaytimeout 3000   //milliseconds
#define inactivitytimeout 5 //minutes
TFT_eSPI tft = TFT_eSPI(tftheight, tftwidth);

/*
  Screen no.        Task
  -1                Off
  1                 Time
  2                 Steps
  3                 Calories
  4                 DEBUGGING Bluetooth logging
  5                 DEBUGGING Accel values
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//screens related
int screen = 1, lastscreen = 1;
#define no_screens 5
//bool needtoupdate = 1;

//Time related
//screen 1 - time
struct tm timeinfo;
int lasthour = 0, lastsec = 0;

//Accelerometer related
//screen 2 - steps
const int MPU = 0x68;
RTC_DATA_ATTR int bootCount = 0, stepstoday = 0, laststepstoday = 0, wakereason = -1;
RTC_DATA_ATTR float calburntoday = 0, lastcalburntoday;
RTC_DATA_ATTR uint32_t millispent;
//Battery related
float vBat;
int active = 0;

//BT logging related
#define bluetoothLogging
#ifdef bluetoothLogging
#include "BluetoothSerial.h"  //Header File for Serial Bluetooth, will be added by default into Arduino
BluetoothSerial ESP_BT;       //Object for Bluetooth
int btstatus = 2, lastbtstatus = 10;
int incoming;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  printWakeReason();
  buttonssetup();
  tftsetup();
  Wire.begin();
  accelAwake(1);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
  batterycheck();
  syncWiFi();

#ifdef bluetoothLogging
  btsetup();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  //go through loop around 40 times a second
  delay(10);
  //  loopscount();
  updatescreen();
  read_Accelerometer();
  toggleButton2();
}
