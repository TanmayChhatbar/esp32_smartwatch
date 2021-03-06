/*
  Commands related to the accelerometer
*/

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, TmP;
int16_t lastAcX, lastAcY, lastAcZ, lastGyX, lastGyY, lastGyZ;
int16_t sumAcXY, lastsumAcXY;
int32_t GyNet, GyNetMax = 0;
int32_t AcNet, AcNetMax = 0;
int noise = 80;
int awake = 1;
String temperature;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_Accelerometer() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint16_t)MPU, (uint8_t)14, true);     //declared types to prevent warning
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  TmP = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  filter(15);               //x% weight given to latest reading

  GyNet = sqrt(sq(GyX) + sq(GyY) + sq(GyZ));                //normalise in all directions
  AcNet = sqrt(sq(AcX) + sq(AcY) + sq(AcZ));                //normalise in all directions
  sumAcXY = AcX + AcY;
  printInSerial('n');     //g for gyro, a for accelerometer, b for both, t for angles

  isActive();           //should watch switch itself off
  updateLast();         //update the 'last' values from accelerometer

  if (((AcZ < -13500 and AcY > 600) and active) or screen >= 4) {   //switch on the display if someone is looking at it
    backlight(1);
  }
  else if (!active or (AcZ > -13500 and AcY < 600)) {
    backlight(0);
  }

  //  isRunning();
  isWalking();
#ifdef bluetoothLogging
  sendBT();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isActive() {
  if (AcX - lastAcX < noise and AcY - lastAcY < noise and AcZ - lastAcZ < noise) {
    if (millis() - activetimer > displaytimeout) {
      active = 0;
      accelPowerMode(0);
    }
    if (millis() - activetimer > (inactivitytimeout))
      gotosleep(1);
  }
  else {
    awake = 1;
    active = 1;
    activetimer = millis();
    accelPowerMode(1);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printInSerial(char sel) {
  if (sel == 'a' or sel == 'b') {
    Serial.print("aX = "); Serial.print(AcX);
    Serial.print("\taY = "); Serial.print(AcY);
    Serial.print("\taZ = "); Serial.print(AcZ);
    Serial.print("\tAcNet = "); Serial.print(AcNet);
  }
  if (sel == 'g' or sel == 'b') {
    Serial.print("gX= "); Serial.print(GyX);
    Serial.print("\tgY= "); Serial.print(GyY);
    Serial.print("\tgZ= "); Serial.print(GyZ);
  }
  if (sel == 'a' or sel == 'g' or sel == 'b') {
    Serial.println();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void filter(int weight) {                           //weight being weight given to latest reading
  AcX = (AcX * weight + lastAcX * (100 - weight)) / 100;
  AcY = (AcY * weight + lastAcY * (100 - weight)) / 100;
  AcZ = (AcZ * weight + lastAcZ * (100 - weight)) / 100;
  GyX = (GyX * weight + lastGyX * (100 - weight)) / 100;
  GyY = (GyY * weight + lastGyY * (100 - weight)) / 100;
  GyZ = (GyZ * weight + lastGyZ * (100 - weight)) / 100;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateLast() {
  lastAcX = AcX;
  lastAcY = AcY;
  lastAcZ = AcZ;
  lastGyX = GyX;
  lastGyY = GyY;
  lastGyZ = GyZ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void accelAwake(bool wake) {      // if wake is 1, wake up, else sleep
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  if (wake) {
    Wire.write(0);
  }
  else {
    Wire.write(0x40);       //write b01000000 in register 6b
  }
  Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void accelPowerMode(bool powermode) {           //powermode 0 is low power, accel only
  if (powermode) {
    accelAwake(1);    //cycle to 0, sleep to 0
    Wire.beginTransmission(MPU);
    Wire.write(0x6C);
    Wire.write(0);       //write b00110111 in register 6b, put ax, ay, and gyros in sleep mode
    Wire.endTransmission();
  }
  else {
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);
    Wire.write(0x20);       //write b00100000 in register 6b, cycle mode
    Wire.endTransmission();

    Wire.beginTransmission(MPU);
    Wire.write(0x6C);
    Wire.write(0x37);       //write b00110111 in register 6c, put ax, ay, and gyros in sleep mode, cycle at 1.25hz
    Wire.endTransmission();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintTemperature() {
  tft.setTextDatum(BR_DATUM);
  tft.setTextColor(textColor, backgroundColor);
  temperature = String((TmP) / 340 + 36.53);
  temperature += "C ";
  tft.drawString(temperature, tftwidth, tftheight, 2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DebuggingScreens
#define valx 100
#define vertspacing 30

//If values show 0, thats probably because of the module being in sleep mode. Move the watch to get it out of sleep.

void TFTPrintAccel() {
  tft.setTextPadding(40);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(String("AcX="), 10, 10, 4);
  tft.drawString(String(String(AcX) + "     "), valx, 10, 4);
  tft.drawString(String("AcY="), 10, 10 + (vertspacing * 1), 4);
  tft.drawString(String(String(AcY) + "     "), valx, 10 + (vertspacing * 1), 4);
  tft.drawString(String("AcZ="), 10, 10 + (vertspacing * 2), 4);
  tft.drawString(String(String(AcZ) + "     "), valx, 10 + (vertspacing * 2), 4);
  tft.drawString(String("AcNet="), 10, 10 + (vertspacing * 3), 4);
  tft.drawString(String(String(AcNet) + "     "), valx, 10 + (vertspacing * 3), 4);
  tftupdate = 1;
}

void TFTPrintGyro() {
  tft.setTextPadding(40);
  tft.setTextDatum(TL_DATUM);
  tft.drawString(String("GyX="), 10, 10, 4);
  tft.drawString(String(String(GyX) + "     "), valx, 10, 4);
  tft.drawString(String("GyY="), 10, 10 + (vertspacing * 1), 4);
  tft.drawString(String(String(GyY) + "     "), valx, 10 + (vertspacing * 1), 4);
  tft.drawString(String("GyZ="), 10, 10 + (vertspacing * 2), 4);
  tft.drawString(String(String(GyZ) + "     "), valx, 10 + (vertspacing * 2), 4);
  tft.drawString(String("GyNet="), 10, 10 + (vertspacing * 3), 4);
  tft.drawString(String(String(GyNet) + "     "), valx, 10 + (vertspacing * 3), 4);
  tftupdate = 1;
}

#endif
