/*
   GyNet vals
   brisk walking 6000-20000
   jogging 13000-30000

   type of exercise           calorie/min       calorie/sec       GyNet
   moderate walking           2.5               .04               a
   brisk walking              4                 .066              6000-20000
   jogging                    11                .183              13000-30000
   sprinting                  20                .333              a


*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GyMin 13000   //for detection of step
#define GyMax 20000   //needs to be variable based on GyNet value, so that walking, jogging, running can be logged
#define AcMin 14000   //for detection of step
#define AcMax 22000   //needs to be variable based on GyNet value, so that walking, jogging, running can be logged
#define maxStepTime 1000  //milliseconds
#define minStepTime 300
unsigned int steptimer, stepDuration;     //timer is overall, duration is millis - timer
bool isstep, lastisstep; //has started calc for a step

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void isWalking() {
  GyNet = sqrt(sq(GyX) + sq(GyY) + sq(GyZ));                //normalise in all directions
  AcNet = sqrt(sq(AcX) + sq(AcY) + sq(AcZ));                //normalise in all directions
//  Serial.print("\tAcNet = ");
//  Serial.print(AcNet);
  if ((AcZ > -15000 and  GyNet < GyMin) or isstep == 1 ) {    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isstep = 1;
    if (lastisstep == 0) {                                  //if new step is expected, start timer
      steptimer = millis();
    }
    if (GyNet > GyNetMax) {
      GyNetMax = GyNet;
    }
    stepDuration = millis() - steptimer;

    if (stepDuration < maxStepTime) {                       //if max time per step hasnt elapsed
      if ((GyNet > GyMax and GyNet < GyNetMax) and stepDuration > minStepTime ) { //if between max and min step times, gynet exceeds gymax, increment steps

        Serial.println(GyNetMax);
        if (GyNetMax < 8000)
          calburntoday += .04 * stepDuration / 1000;
        else if (GyNetMax < 20000)
          calburntoday += .066 * stepDuration / 1000;
        else if (GyNetMax < 30000)
          calburntoday += .183 * stepDuration / 1000;
        else calburntoday += .333 * stepDuration / 1000;

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

/*
void isWalking() {
  GyNet = sqrt(sq(GyX) + sq(GyY) + sq(GyZ));                //normalise in all directions
  AcNet = sqrt(sq(AcX) + sq(AcY) + sq(AcZ));                //normalise in all directions
  Serial.print("\tAcNet = ");
  Serial.println(AcNet);
  if ((AcZ > -15000 and (AcNet < AcMin or GyNet < GyMin)) or isstep == 1 ) {    //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isstep = 1;
    if (lastisstep == 0) {                                  //if new step is expected, start timer
      steptimer = millis();
    }
    if (GyNet > GyNetMax) {
      GyNetMax = GyNet;
    }
    stepDuration = millis() - steptimer;

    if (stepDuration < maxStepTime) {                       //if max time per step hasnt elapsed
      if (((GyNet > GyMax and GyNet < GyNetMax) or AcNet > AcMax) and stepDuration > minStepTime ) { //if between max and min step times, gynet exceeds gymax, increment steps

        Serial.println(GyNetMax);
        if (GyNetMax < 8000)
          calburntoday += .04 * stepDuration / 1000;
        else if (GyNetMax < 20000)
          calburntoday += .066 * stepDuration / 1000;
        else if (GyNetMax < 30000)
          calburntoday += .183 * stepDuration / 1000;
        else calburntoday += .333 * stepDuration / 1000;

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
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void isRunning() {
  //TODO
  //ay < -14000
  //ax 2000-4000
  //acnet 14000-25000
}
