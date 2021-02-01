/*
   Being worked on by Tanmay Chhatbar https://github.com/TanmayChhatbar/esp32_smartwatch
   Written for the TTGO T-Display
   Partition scheme (Huge App) || (Minimal SPIFFS) || [basically any with >=1.5MB App]
   OTA hasnt yet been inculcated because WiFi is switched off after fetching time to conserve battery life
   
   v0.21
   Screen no.        Task                       Type
   -1                Off                        Off

    1                 Current Time              Basic
    2                 Steps count               Basic
    3                 Calories burned           Basic

    4                 Bluetooth logging status  Debug
    5                 Accel values              Debug
    6                 Gyro values               Debug


    To do:
    Running code test
    Steps counter threshold - check variable stepsThreshold
    OTA - keep WiFi on for a while after boot
    Check if battery is charging
    Music control
    FreeRTOS - ref https://www.youtube.com/watch?v=NEq-L9TNMts
*/

#include "librariesused.h"  //list of libraries used
#include "tftdefinition.h"  //definition of T-Display onboard display and buttons

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BT logging related
#define bluetoothLogging      //comment to remove bluetooth logging, needs to be before featuredefinition.h

//screens related
#define basic_screens 3       //basic screens are those excluding debugging/logging screens
#define DebuggingScreens      //commment to remove debugging screens, needs to be before featuredefinition.h
#include"featuredefinition.h"
int screen = 1, lastscreen = 1;

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
unsigned long activetimer = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  SerialprintWakeReason();
  tftsetup();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
  batterycheck();
  Wire.begin();
  accelAwake(1);
  syncWiFi();

#ifdef bluetoothLogging
  btsetup();
#endif
  buttonssetup();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(10);
  //    loopscount();   //uncomment to count frequency of loop
  updatescreen();
  read_Accelerometer();
  toggleButton2;
}
