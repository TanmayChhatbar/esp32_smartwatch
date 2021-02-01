/*
   Commands related to updating the screen
*/
//TFT Colors
#define backgroundColor TFT_BLACK
#define textColor TFT_WHITE
#define batteryColor TFT_WHITE
#define timeColor TFT_GREEN

//set correct screen numbers based on bluetooth/debugging screen definitions

bool tftupdate = 1;         //update without refreshing entire screen
bool tftfullupdate = 1;     //draw entire screen as background before updating
bool backlightState = 1;
String timespent;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tftsetup() {
  tft.begin();
  tft.setRotation(1); //Landscape
  tft.fillScreen(backgroundColor);
  tft.setTextColor(textColor, backgroundColor);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updatescreen() {
  if (screen != lastscreen or tftfullupdate) {
    tft.fillScreen(backgroundColor);        //if the screen needs to be changed, write the screen all black
    tft.setTextColor(textColor, backgroundColor);
    active = 1;
  }
  if (tftfullupdate) {
    tftfullupdate = 0;
    tftupdate = 1;
    activetimer = millis();
  }
  switch (screen) {                   //update display loop
    case -1:                          //-1 indicates display off, dont refresh screen
      tftupdate = 0;
      break;

    case p1:                          //show time
      TFTPrintTime();                 //update part of screen every second
      break;

    case p2:                          //show steps
      TFTPrintSteps();
      break;

    case p3:                          //calories
      TFTPrintCalories();
      break;

      //DEBUGGING

#ifdef bluetoothLogging
    case p4:
      TFTPrintBT();    //in btlog.ino file
      break;
#endif

#ifdef DebuggingScreens
    case p5:
      TFTPrintAccel();    //in gy521.ino file
      break;

    case p6:
      TFTPrintGyro();    //in gy521.ino file
      break;
#endif
  }
  if (screen > 0) {
    batterydraw();
    lastscreen = screen;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void backlight(bool state) {          //call if want to change state of backlight
  if (backlightState != state) {
    tftupdate = 1;
    backlightState = state;
  }
  digitalWrite(TFT_BL, state);
  if (!state) {
    screen = -1;
  }
  else screen = lastscreen;
}
