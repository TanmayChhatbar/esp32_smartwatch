/*
  Commands related to fetching and keeping time
*/

#include <credentials.h>
unsigned long timerWiFi = 0;

const char* ntpServer = "time1.google.com";
const long  gmtOffset_sec = 19800;    //+5:30
const int   daylightOffset_sec = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void syncWiFi() {       //connects to WiFi, and fetches time
y:
  Serial.printf("Connecting to %s", ssid);
  //  tft.setFreeFont(&FreeMono9pt7b);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("Connecting to ", 0, 0, 4);
  tft.drawString(ssid, 0, 30, 4);
  WiFi.begin(ssid, password);
  timerWiFi = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (millis() - timerWiFi > 10000)
    {
      goto y;
    }
  }
  String ip = "IP: " + (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1];
  ip += "." + (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3];
  
  tft.drawString(ip, 0, 60, 4);
  Serial.println("");
  Serial.print("WiFi Connected\nIP Address: ");
  Serial.println(WiFi.localIP());
  delay(50);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!getLocalTime(&timeinfo)) {
    delay(100);
  }
  lasthour = timeinfo.tm_hour;

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(50);
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
