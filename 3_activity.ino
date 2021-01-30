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

unsigned int steptimer, stepDuration;     //timer is overall, duration is millis - timer
bool isstep, lastisstep, isrunstep, lastisrunstep; //has started calc for a step

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isRunning() {
  //if sum has reached minima, and is below -1000, probably running
  if ((AcZ > -15000 and  sumAcXY < -10000 and sumAcXY > lastsumAcXY) or isrunstep == 1 ) {   
    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isrunstep = 1;
    if (lastisrunstep == 0) {                                  //if new step is expected, start timer
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

        millispent += stepDuration;
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
  if (isrunstep == 0 and ((AcZ > -15000 and  GyNet < GyMin) or isstep == 1 )) {   
    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isstep = 1;
    if (lastisstep == 0) {                                  
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
