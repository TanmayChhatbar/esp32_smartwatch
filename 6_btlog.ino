/*
  Commands related to bluetooth logging
*/

#ifdef bluetoothLogging

int inbuffer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void btsetup() {
  ESP_BT.begin("ESP32_DataLog"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  tft.drawString("BT Ready to Pair", 0, 90, 4);
  delay(250);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sendBT() {
  if (ESP_BT.available()) { //Check if we receive anything from Bluetooth
    inbuffer = ESP_BT.read();
    if (inbuffer > 47 and inbuffer < 50)
      incoming = inbuffer;
  }
  //  Serial.println(btstatus);
  if (incoming == 49) {
    printinBT();
    btstatus = 1; //sending
  }
  else btstatus = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printinBT() {
  { //acceleration values
    ESP_BT.print("A:");
    ESP_BT.print(AcX);
    ESP_BT.print(",");
    ESP_BT.print(AcY);
    ESP_BT.print(",");
    ESP_BT.print(AcZ);
    ESP_BT.print(",");
    ESP_BT.print(AcNet);
    ESP_BT.print(",");
  }
  { //gyro values
    ESP_BT.print("  G:");
    ESP_BT.print(GyX);
    ESP_BT.print(",");
    ESP_BT.print(GyY);
    ESP_BT.print(",");
    ESP_BT.print(GyZ);
    ESP_BT.print(",");
    ESP_BT.print(GyNet);
    ESP_BT.print(",");
    ESP_BT.print(GyNetMax);
    ESP_BT.print(",");
  }
  //  { //steps
  //    ESP_BT.print(stepstoday);
  //    ESP_BT.print(",");
  //  }
  { //step change
    ESP_BT.print("  S:");
    if (laststepstoday != stepstoday)
      ESP_BT.print("1");
    else ESP_BT.print("0");
    ESP_BT.print(",");
  }
  ESP_BT.println();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintBT() {
  tft.setTextDatum(TL_DATUM);
  tft.setTextPadding(20);

  tft.drawString("BT Status", 10, 10, 4);
  if (btstatus != lastbtstatus or tftupdate) {
    lastbtstatus = btstatus;
    switch (btstatus) {
      case 0:
        tft.drawString("Stopped", 10, 70, 4);
        break;
      case 1:
        tft.drawString("Sending", 10, 70, 4);
        break;
      case 2:
        tft.drawString("To pair", 10, 60, 4);
        break;
    }
    tftupdate = 0;
  }
}

#endif
