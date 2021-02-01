/*
   GyNet vals
   brisk walking 6000-20000
   jogging 13000-30000

   type of exercise           calorie/min       calorie/sec       GyNet           AyNet
   moderate walking           2.5               .04               a               a
   brisk walking              4                 .066              7000-20000      -noisy-
   jogging                    11                .183              13000-30000     20000-30000
   sprinting                  20                .333              a               a


*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//walking
#define GyMin 8000   //for detection of step, before test val 13000
#define GyMax 18500   //needs to be variable based on GyNet value, so that walking, jogging, running can be logged, before test val 20000

//running
#define AcMin 20000   //for detection of step
#define AcMax 30000   //needs to be variable based on GyNet value, so that walking, jogging, running can be logged

#define maxStepTime 500 //milliseconds, prev 1000
#define minStepTime 125     //prev 300

#define stepsThreshold 10 //TODO
#define stepsThresholdTime (stepsThreshold*1.5) * maxStepTime * 2

unsigned int steptimer, stepDuration;     //timer is overall, duration is millis - timer
bool isstep, lastisstep, isrunstep, lastisrunstep; //has started calc for a step

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isRunning() {
  //if sum has reached minima, and is below -1000, probably running
  if ((AcZ > -15000 and  sumAcXY < -10000 and sumAcXY > lastsumAcXY) or isrunstep) {
    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isrunstep = 1;
    if (!lastisrunstep) {                                  //if new step is expected, start timer
      steptimer = millis();
    }
    if (AcNet > AcNetMax) {
      AcNetMax = AcNet;
    }
    stepDuration = millis() - steptimer;

    if (stepDuration < maxStepTime) {                       //if max time per step hasnt elapsed
      if ((AcNet > AcMax and AcNet < AcNetMax) and stepDuration > minStepTime ) {
        //if between max and min step times, gynet exceeds gymax, increment steps
        if (AcNetMax < 20000)
          calburntoday += .066 * stepDuration / 1000;
        else if (AcNetMax < 30000)
          calburntoday += .183 * stepDuration / 1000;
        else calburntoday += .333 * stepDuration / 1000;

        millispent += 2 * stepDuration;                       //assuming both half-steps take the same time
        stepstoday++;
        isrunstep = 0;                                         //incremented, wait for
      }
    }
    else {
      isrunstep = 0;
      steptimer = millis();
      AcNetMax = 0;
    }
    lastisrunstep = isrunstep;
  }
  lastsumAcXY = sumAcXY;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isWalking() {
  if (!isrunstep and ((AcZ > -15000 and  GyNet < GyMin) or isstep )) {
    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isstep = 1;
    if (!lastisstep) {
      //if new step is expected, start timer
      steptimer = millis();
    }
    if (GyNet > GyNetMax) {
      GyNetMax = GyNet;
    }
    stepDuration = millis() - steptimer;

    if (stepDuration < maxStepTime) {                       //if max time per step hasnt elapsed
      if ((GyNet > GyMax and GyNet < GyNetMax) and stepDuration > minStepTime ) {
        //if between max and min step times, gynet exceeds gymax, increment steps

        Serial.println(GyNetMax);
        if (GyNetMax < 8000)
          calburntoday += .04 * stepDuration / 1000;
        else if (GyNetMax < 20000)
          calburntoday += .066 * stepDuration / 1000;
        else if (GyNetMax < 30000)
          calburntoday += .183 * stepDuration / 1000;
        else calburntoday += .333 * stepDuration / 1000;
        millispent += stepDuration;
        stepstoday++;
        isstep = 0;                                         //incremented, wait for
      }
    }
    else {
      isstep = 0;
      steptimer = millis();
      GyNetMax = 0;
    }
    lastisstep = isstep;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String minspent, secspent;
void TFTPrintSteps() {                                              //print the steps on the display
  if (screen != lastscreen or stepstoday != laststepstoday or tftupdate) {
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(20);
    tft.drawString("Steps today", 0, 0, 4);                         //#4 RLE font
    tft.drawString((String)stepstoday, 0, 30, 8);                   //#8 RLE font
    tft.setTextDatum(BL_DATUM);
    timespent = ((millispent / 60000) >= 1) ? (String(millispent / 60000) + "min, ") : "";
    timespent += String((millispent / 1000) % 60) + "sec";
    tft.drawString(timespent, 0, tftheight, 4);
    laststepstoday = stepstoday;
    tftupdate = 0;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TFTPrintCalories() {              //print the calories on the display
  if (screen != lastscreen or calburntoday != lastcalburntoday or tftupdate) {
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(20);
    tft.drawString("Calories today", 0, 10, 4);   //#4 RLE font
    tft.drawString((String)calburntoday, 0, 40, 4);   //#8 RLE font
    tftupdate = 0;
  }
}
