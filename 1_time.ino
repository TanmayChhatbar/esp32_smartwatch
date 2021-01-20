/*
  Commands related to fetching and keeping time
*/
//screen 1 - time
const char* ssid = "ssidhere";
const char* password   = "passwordhere";
unsigned long timerWiFi = 0;

const char* ntpServer = "time1.google.com";
const long  gmtOffset_sec = 19800;    //+5:30
const int   daylightOffset_sec = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void syncWiFi() {       //connects to WiFi, and fetches time
y:
  Serial.printf("Connecting to %s", ssid);
  tft.setFreeFont(&FreeMono9pt7b);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Connecting to ", 120, 20);
  tft.drawString(ssid, 120, 40);
  WiFi.begin(ssid, password);
  timerWiFi = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - timerWiFi > 10000)
    {
      goto y;
    }
  }
  String ip = (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1] + "." + (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3];
  tft.drawString(ip, 120, 60);
  Serial.println("");
  Serial.print("WiFi Connected\nIP Address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!getLocalTime(&timeinfo)) {
    delay(100);
  }
  lasthour = timeinfo.tm_hour;

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  tft.fillScreen(TFT_BLACK);
  delay(100);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int serialPrintLocalTime() {          //serial print current time, returns 1 if error
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");////////////////
    TFTPrintTime(1);
    syncWiFi();
    return 1;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return 0;
}
