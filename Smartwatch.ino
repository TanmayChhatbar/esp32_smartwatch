/*
 * Being worked on by Tanmay Chhatbar
 * v0.21
 * Screen no.        Task                       Type   
 * -1                Off                        Off   
 *    
 *  1                 Time                       Basic   
 *  2                 Steps                      Basic   
 *  3                 Calories                   Basic
 *       
 *  4                 Bluetooth logging status   Debug   
 *  5                 Accel values               Debug   
 *  6                 Gyro values                Debug
 *  
 *  
 *       features to add    
 *       music control
*/
//FreeRTOS reference https://www.youtube.com/watch?v=NEq-L9TNMts
#include "librariesused.h"
#include "tftdefinition.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BT logging related
#define bluetoothLogging      //comment to remove bluetooth logging, needs to be before screensdefinition.h
#ifdef bluetoothLogging
#include "BluetoothSerial.h"  //Header File for Serial Bluetooth, will be added by default into Arduino
BluetoothSerial ESP_BT;       //Object for Bluetooth
int btstatus = 2, lastbtstatus = 10;
int incoming;
#endif

//screens related
#define basic_screens 3   //basic screens are those excluding debugging/logging screens
#define DebuggingScreens      //commment to remove debugging screens, needs to be before screensdefinition.h
#include"screensdefinition.h"
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
  //  loopscount();   //uncomment to count frequency of loop
  updatescreen();
  read_Accelerometer(); 
  toggleButton2;
}
