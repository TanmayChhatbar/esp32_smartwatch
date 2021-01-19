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
#define tftwidth 135
#define tftheight 240
#define displaytimeout 3000   //milliseconds
#define inactivitytimeout 5 //minutes
TFT_eSPI tft = TFT_eSPI(tftwidth, tftheight);

/*
  Screen no.        Task
  -1                Off
  1                 Time
  2                 Steps
  3                 Calories
  4                 Music
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//screens related
int screen = 1, lastscreen = 1;
#define no_screens 4
bool backlightState = 1;
//Time related
//screen 1 - time
struct tm timeinfo;
int lasthour = 0, lastsec = 0;

//Accelerometer related
//screen 2 - steps
const int MPU = 0x68;
RTC_DATA_ATTR int bootCount = 0, stepstoday = 0, laststepstoday = 0, wakereason = -1;
RTC_DATA_ATTR float calburntoday = 0, lastcalburntoday;
//Battery related
float vBat;
int active = 0;


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

  pinMode(BUTTON1PIN, INPUT);
  pinMode(BUTTON2PIN, INPUT);
  attachInterrupt(BUTTON1PIN, toggleButton1, FALLING);
  //  attachInterrupt(BUTTON2PIN, toggleButton2_t, FALLING);

  //  delayfunc("tft", 5);
  tft.begin();
  tft.setRotation(1); //Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // delayfunc("accelerometer", 5);
  Wire.begin();
  accelAwake(1);

  //deep sleep
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);

  vBat = analogRead(34) * 0.001772;
  if (vBat < 3.3) {
    gotosleep(0);
  }

  //  delayfunc("wifi", 5);
  syncWiFi();

#ifdef bluetoothLogging
  //  delayfunc("bt", 5);
  btsetup();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(10);                          //go through loop around 40 times a second
  //  loopscount();
  updatescreen();
  read_Accelerometer();
  toggleButton2();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printWakeReason() {
  switch (wakereason) {
    case -1:
      Serial.println("First boot.");
      break;
    case 0:
      Serial.println("Had slept of low battery.");
      break;
    case 1:
      Serial.println("Had slept from inactivity.");
      break;
    case 2:
      Serial.println("Was put to sleep manually.");
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gotosleep(int reason) {     //1 is inactivity, 0 is low battery, 2 is user off
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  wakereason = reason;
  switch (reason) {
    case 0:
      tft.drawString("Low battery", 120, 60, 4);
      break;
    case 1:
      tft.drawString("Inactivity timeout", 120, 60, 4);
      break;
    default:
      tft.drawString("Switching off", 120, 60, 4);
      break;
  }
  Serial.print("Putting accelerometer to sleep... ");
  //put mpu6050 in sleep mode
  accelAwake(0);
  Serial.println("Done");
  delay(100);

  Serial.println("Going to sleep now");
  int sleepbugtimer = millis();
prev:
  if (millis() - sleepbugtimer < 2500) {
    goto prev;
  }
  else {
    esp_deep_sleep_start();
  }
  //save number of steps today. done with RTC_DATA_ATTR
  //sleep until interrupt by toggleButton2
}
