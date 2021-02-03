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
#define GyMin 6500          //for detection of step, before test val 13000
#define GyMinMin 1200       //under this, loop wont be entered
#define GyMax 11000         //needs to be variable based on GyNet value, so that walking, jogging, running can be logged, before test val 20000
#define GyNetMaxMax 24500   //if GyNetMax exceeds this value, step wont be counted
//running
#define AcMin 20000         //for detection of step
#define AcMax 30000         //needs to be variable based on GyNet value, so that walking, jogging, running can be logged

#define maxStepTime 600     //milliseconds, prev 1000
#define minStepTime 300     //prev 300

//definitions for movement start counter only after threshold number of steps have taken place, so normal movement doesnt accidentally count as moving
#define stepsThreshold 10 //10 steps in threshold time to get into moving mode
#define stepsThresholdTime (stepsThreshold * maxStepTime * 2)   // 12 seconds, to get into moving mode
#define stepsThresholdTimeout (stepsThreshold * maxStepTime * 2)// 12 seconds, to get out of moving mode

/*
   #definitions
   steptimer - used to calculate stepduration
   stepDuration - duration of current step, will be used to check for noise by comparing with max/min step times
   movingTimer - used to detect initial movement, will be compared with threshold time to understand consistent movement
   movingSteps - buffer steps for reducing noise
   stepsThreshold - minimum steps in stepsthresholdtime to register consistent movement
   stepsThresholdTime - read previous
   stepsThresholdTimeout - max time taken to register threshold number of steps

   bool variables
   isstep/islaststep - used to remember state of whether step is expected
   isrunstep/islastrunstep - used to remember state of whether running step is expected
   isMoving/lastisMoving - used to ascertain whether consistent movement was detected
*/
unsigned int steptimer, stepDuration, movingTimer, movingSteps;            //timer is overall, duration is millis - timer
bool isstep, lastisstep, isrunstep, lastisrunstep;

bool isMoving, lastisMoving;  //has started calc for a step

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isWalking() {
  if (!isrunstep) {                                             //if running was detected, skip entire loop
    if ((AcZ > -15000 and  GyNet < GyMin and GyNet > GyMinMin) or isstep) {    //if not running, and new step is detected to start, or if step is expected.
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
            printout("New Step counted");
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
