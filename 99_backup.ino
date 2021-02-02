/*
  //fallback
  void isWalking() {
  GyNet = sqrt(sq(GyX) + sq(GyY) + sq(GyZ));                //normalise in all directions
  AcNet = sqrt(sq(AcX) + sq(AcY) + sq(AcZ));                //normalise in all directions
  Serial.print("\tAcNet = ");
  Serial.println(AcNet);
  if ((AcZ > -15000 and (AcNet < AcMin or GyNet < GyMin)) or isstep ) {
  //if new step is detected to start, or if step is expected. AcZ value so it doesnt trigger when face up
    isstep = 1;
    if (!lastisstep) {                                  //if new step is expected, start timer
      steptimer = millis();
    }
    if (GyNet > GyNetMax) {
      GyNetMax = GyNet;
    }
    stepDuration = millis() - steptimer;

    if (stepDuration < maxStepTime) {                       //if max time per step hasnt elapsed
      if (((GyNet > GyMax and GyNet < GyNetMax) or AcNet > AcMax) and stepDuration > minStepTime ) {
      //if between max and min step times, gynet exceeds gymax, increment steps

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
