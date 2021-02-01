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
    Serial.println("Failed to obtain time");
    TFTPrintTime();
    syncWiFi();
    return 1;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintTime() {               //print the time on the display
  getLocalTime(&timeinfo);          //update time
  if (timeinfo.tm_sec != lastsec or tftupdate) {       //update part of screen every second
    lastsec = timeinfo.tm_sec;

    tft.setTextDatum(MC_DATUM);
    tft.setTextPadding(240);
    tft.setTextColor(timeColor, backgroundColor);

    //only hour and minute, with blinking colon
    String timeprint = timeinfo.tm_hour % 12 == 0 ? "12" : (timeinfo.tm_hour % 12 < 10 ? "0" : "") + (String)(timeinfo.tm_hour % 12);
    timeprint += (timeinfo.tm_sec % 2 == 1) ? ":" : " ";
    timeprint += (timeinfo.tm_min < 10 ? "0" : "") + (String)timeinfo.tm_min;

    tft.drawString(timeprint, 120, 60, 7);  //7 is digital font from RLE fonts
    tftupdate = 0;
    TFTPrintTemperature();    //in gy521.ino file
  }
}
