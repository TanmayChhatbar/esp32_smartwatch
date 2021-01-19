/*
  Commands related to the accelerometer
*/

int16_t AcX, AcY, AcZ, GyX, GyY, GyZ, TmP;
int16_t lastAcX, lastAcY, lastAcZ, lastGyX, lastGyY, lastGyZ;
int32_t GyNet, GyNetMax = 0;
int32_t AcNet, AcNetMax = 0;
unsigned long timer = 0, activetimer = 0;
float thx, thy, thz;
int noise = 100, initialnoise = 200;
int awake = 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_Accelerometer() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  TmP = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  filter(15);               //x% weight given to latest reading
  printInSerial('a');     //g for gyro, a for accelerometer, b for both, t for angles

  isActive();           //should watch switch itself off
  updateLast();         //update the 'last' values from accelerometer

  if ((AcZ < -13500 and AcY > 600) and active == 1)    //switch on the display if someone is looking at it
    backlight(1);
  else if (active == 0 or (AcZ > -13500 and AcY < 600))
    backlight(0);

  isWalking();
  Serial.println();
#ifdef bluetoothLogging
  sendBT();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void printInSerial(char sel) {
  if (sel == 'a' or sel == 'b') {
    Serial.print("aX = "); Serial.print(AcX);
    Serial.print("\taY = "); Serial.print(AcY);
    Serial.print("\taZ = "); Serial.print(AcZ);
  }
  if (sel == 'g' or sel == 'b') {
    Serial.print("gX= "); Serial.print(GyX);
    Serial.print("\tgY= "); Serial.print(GyY);
    Serial.print("\tgZ= "); Serial.print(GyZ);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isActive() {
  if (AcX - lastAcX < noise and AcY - lastAcY < noise and AcZ - lastAcZ < noise) {
    if (millis() - activetimer > displaytimeout) {
      active = 0;
      accelPowerMode(0);
    }
    if (millis() - activetimer > (inactivitytimeout * 60 * 1000))
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
  if (wake == 1) {
    Wire.write(0);
    Wire.endTransmission(true);
  }
  else {
    Wire.write(0x40);       //write b01000000 in register 6b
    Wire.endTransmission();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void accelPowerMode(bool powermode) {           //powermode 0 is low power, accel only
  if (powermode == 1) {
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
