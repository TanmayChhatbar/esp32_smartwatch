#include "definition_activity.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isWalking() {
  if (!isrunstep) {                                             //if running was detected, skip entire loop
    if ((!facingUp and  GyNet < GyMin and GyNet > GyMinMin) or isstep) {    //if not running, and new step is detected to start, or if step is expected.
      //AcZ value so it doesnt trigger when face up
      isstep = 1;
      if (!lastisstep) {                                        //if new step is expected, start timer
        printout("New Step start");
        steptimer = millis();
      }
      if (GyNet > GyNetMax) {                                   //find maxima, dont increment step until maxima is found
        GyNetMax = GyNet;
        printout((String)GyNetMax);
      }
      stepDuration = millis() - steptimer;                      //stepDuration to track time spent moving

      if (stepDuration < maxStepTime) {                         //if max time per step hasnt elapsed
        if ((GyNetMax > GyMax and GyNetMax > GyNet and GyNetMax < GyNetMaxMax) and stepDuration > minStepTime) {
          //if within acceptable step duration, gynet exceeds gymax, and gynet is maximum it has been for this step
          //ASSUME STEP
          if (!isMoving) {                                      //if not sure if moving, check if 10 steps made in 10 seconds
            if (movingSteps == 0) {                             //start timer at first movingStep
              printout("Started movement detect timer");
              movingTimer = millis();
            }
            movingSteps++;
            printout("Initial step counted: " + (String)movingSteps);
            if (movingSteps >= stepsThreshold) {                //if moving steps greater than threshold, count them all, and now are certain of movement
              movingSteps = 0;
              stepstoday += stepsThreshold;
              millispent += (millis() - movingTimer);
              printout("Threshold crossed. All steps counted. Will count as normal now");
              isMoving = 1;
            }
            else if (millis() - movingTimer > stepsThresholdTimeout) //if timed out, reset movingsteps
            {                                                         //TODO NEED TO SHIFT THIS CHECK
              movingSteps = 0;
              printout("Movement start timeout.");
            }
          }
          else {                                                //if already certain of movement, increment steps as usual
            calorieCount(0);
            millispent += 2 * stepDuration;
            stepstoday++;
            printout("New Step counted: " + (String)stepstoday);
          }
          isstep = 0;                                           //incremented, wait for
          GyNetMax = 0;
        }
        else if (GyNetMax > GyNetMaxMax) {                      //TEST
          isstep = 0;
          GyNetMax = 0;
        }
      }
      else {                                                    //if max time for the step has elapsed, ignore current potential step
        isstep = 0;
        GyNetMax = 0;
      }
      lastisstep = isstep;
      lastisMoving = isMoving;
    }
    else if (GyNet < GyMinMin) {                                                //will trigger when face up, or GyNet value outside walking range, and movement very little
      notMoving();
      lastisMoving = 0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isRunning() {
  //if sum has reached minima, and is below -1000, probably running
  if ((!facingUp and  sumAcXY < -10000 and sumAcXY > lastsumAcXY) or isrunstep) {
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
        calorieCount(1);

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

void notMoving() {
  if (isMoving) {                                         //and if last known state was moving
    if (lastisMoving) {                                   //start timer at first loop of here
      printout("Movement timeout timer started");
      movingTimer = millis();
    }
    lastisMoving = 0;
    if (millis() - movingTimer > stepsThresholdTimeout) {
      printout("Movement timed out");
      isMoving = 0;
      movingSteps = 0;
    }
  }
  isstep = 0;
  GyNetMax = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void calorieCount(bool walkrun) {     //0 is walk, 1 is run
  if (walkrun) {
    if (GyNetMax < 8000)
      calburntoday += .04 * stepDuration * 2 / 1000;
    else if (GyNetMax < 20000)
      calburntoday += .066 * stepDuration * 2 / 1000;
    else if (GyNetMax < 30000)
      calburntoday += .183 * stepDuration * 2 / 1000;
    else calburntoday += .333 * stepDuration * 2 / 1000;
  }
  else {
    if (AcNetMax < 20000)
      calburntoday += .066 * stepDuration / 1000;
    else if (AcNetMax < 30000)
      calburntoday += .183 * stepDuration / 1000;
    else calburntoday += .333 * stepDuration / 1000;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String minspent, secspent;
void TFTPrintSteps() {                                              //print the steps on the display
  if (screen != lastscreen or stepstoday != laststepstoday or tftupdate) {
    tft.setTextDatum(TL_DATUM);
    tft.setTextPadding(20);
    if (isMoving)
      tft.setTextColor(walkingColor, backgroundColor);            ///test
    tft.drawString("Steps today", 0, 0, 4);                         //#4 RLE font
    if (stepstoday < 10000)                                         //if steps into 5 digits, reduce font size
      tft.drawString((String)stepstoday, 0, 30, 8);                 //#8 RLE font
    else
      tft.drawString((String)stepstoday, 0, 30, 4);                 //#8 RLE font
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
