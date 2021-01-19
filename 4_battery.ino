/*
  Commands related to the battery
*/
//variables to set
#define clearance 0    //distance from top right corner on both x and y
#define totalheight 8
#define boopheight 4   //height of the boop in the end, try to keep so totalheight - boopheight is even
#define boopwidth 2
#define divisions 4
#define divisionwidth 3

//variables to not touch
#define totalwidth (2 + boopwidth + (divisions + 1) + divisions * divisionwidth)
#define framewidth (totalwidth - boopwidth)     //frame width
#define tlpointx (240 - clearance - totalwidth)
#define tlpointy (clearance)
#define charging 0
#define boopx (240 - clearance - boopwidth)
#define boopy (tlpointy + (totalheight - boopheight) / 2)
#define divoffset (divisionwidth + 1)

int color, divdraw;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void batterydraw() {            //TODO
  vBat = analogRead(34) * 0.001772;
  if (vBat < 3.3)
    gotosleep(0);
  else if (vBat < 3.4)
    divdraw = 0;
  else if (vBat < 3.6)
    divdraw = 1;
  else if (vBat < 3.8)
    divdraw = 2;
  else if (vBat < 4.05)
    divdraw = 3;
  else divdraw = 4;

  if (charging)
    color = TFT_GREEN;                           //test charging color change
  else
    color = TFT_WHITE;

  tft.drawRect(tlpointx, tlpointy, framewidth, totalheight, color);      //main frame
  tft.fillRect(boopx, boopy, boopwidth, boopheight, color);     //boop
  tft.fillRect(tlpointx + 1, tlpointy + 1, framewidth - 2, totalheight - 2, TFT_BLACK);      //reset to 0 battery
  for ( int i = 0; i < divdraw; i++) {
    tft.fillRect(tlpointx + 2 + divoffset * i, tlpointy + 2, divisionwidth , totalheight - 4, color);      //smaller rectangles inside frame
  }

  tft.setTextDatum(BR_DATUM);
  tft.drawString(String(vBat), 240, 135, 2);            //test
}
