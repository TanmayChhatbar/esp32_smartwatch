/*
  Commands related to power management and sleep
*/

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
  //sleep until interrupt by toggleButton1
}
