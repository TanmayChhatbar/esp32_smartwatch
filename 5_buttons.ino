/*
  Commands related to onboard buttons connected as interrupts
*/

long unsigned timer1 = 0, timer2 = 0;         //timers to debounce onboard buttons
int debounceTime = 300;//ms

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggleButton1() {
  if (millis() - timer1 > debounceTime)
  {
    timer1 = millis();
    screen++;
    if (screen <= 0)
      screen = lastscreen;
    else if (screen > no_screens)
      screen = 1;
    Serial.print("Screen ");
    Serial.println(screen);
    tft.fillScreen(TFT_BLACK);
    active = 1;
    tftupdate = 1;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggleButton2() {
  timer2 = millis();
  while (digitalRead(BUTTON2PIN) == LOW) {
    Serial.println("button2");
    if (millis() - timer2 > 1000) {
      gotosleep(2);
    }
    delay(100);
  }
}
