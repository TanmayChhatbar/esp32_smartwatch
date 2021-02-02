/*
   Being worked on by Tanmay Chhatbar https://github.com/TanmayChhatbar/esp32_smartwatch
   Written for the TTGO T-Display
   Partition scheme (Huge App) || (Minimal SPIFFS) || [basically any with >=1.5MB App]
   OTA hasnt yet been inculcated because WiFi is switched off after fetching time to conserve battery life

   v0.4
   Screen no.        Task                       Type        Current state
   -1                Off                        Off

    1                 Current Time              Basic       Works
    2                 Steps count               Basic       92%-97% accurate while walking, compared to "Step Tracker & Pedometer" app, around 105%-110% of walking time registered
    3                 Calories burned           Basic       experimental, not reliable (30%-35% of expected value from same app)

    4                 Bluetooth logging status  Debug       Works
    5                 Accel values              Debug       Works
    6                 Gyro values               Debug       Works


    TODO:
    Running code test
    OTA - keep WiFi on for a while after boot
    Check if battery is charging
    Music control
    FreeRTOS - ref https://www.youtube.com/watch?v=NEq-L9TNMts
    Long term track of steps
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
const int MPU = 0x68;

//Variables stored in RTC memory
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
  buttonssetup();
  delay(100);
  if (!digitalRead(BUTTON2PIN)) {
    //TODO OTA WIFI, KEEP WIFI ON FOR 30 SECONDS, ALSO NEED TO ADD SETUP FOR LIBRARY
  };

  Wire.begin();
  accelAwake(1);
  syncWiFi();

#ifdef bluetoothLogging
  btsetup();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  delay(10);
  //      loopscount();   //uncomment to count frequency of loop
  updatescreen();
  read_Accelerometer();
  toggleButton2();
}
