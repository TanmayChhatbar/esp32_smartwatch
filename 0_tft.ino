/*
  Commands related to updating the screen
*/
bool tftupdate = 1;
bool needtoupdate = 1;
bool backlightState = 1;
String timespent;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tftsetup() {
  tft.begin();
  tft.setRotation(1); //Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updatescreen() {
  if (screen != lastscreen or needtoupdate == 1) {
    tft.fillScreen(TFT_BLACK);        //if the screen needs to be changed, write the screen all black
  }
  if (needtoupdate == 1) {
    needtoupdate = 0;
    tftupdate = 1;
  }
  switch (screen) {                   //update display loop
    case -1:                          //-1 indicates display off, dont refresh screen
      break;

    case 1:                           //show time
      getLocalTime(&timeinfo);        //update time
      if (timeinfo.tm_sec != lastsec or tftupdate == 1) {
        lastsec = timeinfo.tm_sec;
        TFTPrintTime(0);              //update part of screen every second
      }
      break;

    case 2:                           //show steps
      TFTPrintSteps();
      break;

    case 3:                           //calories
      TFTPrintCalories();
      break;

#ifdef bluetoothLogging
    case 4:
      TFTPrintBT();
      break;
#endif
    case 5:
      TFTPrintAccel();
      break;
  }
  if (screen > 0) {
    batterydraw();
    lastscreen = screen;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintTime(int ret) {              //print the time on the display
  tft.setTextDatum(MC_DATUM);
  tft.setTextPadding(240);

  //only hour and minute, with blinking colon
  String timeprint = (timeinfo.tm_hour % 12 == 0 ? "12" : (timeinfo.tm_hour % 12 < 10 ? "0" : "") + (String)(timeinfo.tm_hour % 12)) + ((timeinfo.tm_sec % 2 == 1) ? ":" : " ") + (timeinfo.tm_min < 10 ? "0" : "") + (String)timeinfo.tm_min;
  tft.drawString(timeprint, 120, 60, 7);  //7 is digital font from RLE fonts
  tftupdate = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String minspent, secspent;
void TFTPrintSteps() {              //print the steps on the display
  if (screen != lastscreen or stepstoday != laststepstoday or tftupdate == 1) {
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(20);
    tft.drawString("Steps today", 0, 0, 4);   //#4 RLE font
    tft.drawString((String)stepstoday, 0, 30, 8);   //#8 RLE font
    tft.setTextDatum(BL_DATUM);
    timespent = ((millispent / 60000) >= 1) ? (String(millispent / 60000) + "min, ") : "";
    timespent += String((millispent / 1000) % 60) + "sec";
    tft.drawString(timespent, 0, tftheight, 4);   //#8 RLE font
    laststepstoday = stepstoday;
    tftupdate = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintCalories() {              //print the calories on the display
  if (screen != lastscreen or calburntoday != lastcalburntoday or tftupdate == 1) {
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(20);
    tft.drawString("Calories today", 0, 0, 4);   //#4 RLE font
    tft.drawString((String)calburntoday, 0, 30, 8);   //#8 RLE font
    tftupdate = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void backlight(bool state) {          //call if want to change state of backlight
  backlightState = state;
  digitalWrite(TFT_BL, state);
  if (state == 0) {
    screen = -1;
  }
  else screen = lastscreen;
  tftupdate = 1;
}

//void TFTPrint3() {
//  tft.setTextPadding(20);
//  tft.setTextDatum(MC_DATUM);
//  tft.drawString("Screen 3", 120, 60);
//  tftupdate = 0;
//  Serial.println("TEST");
//}
