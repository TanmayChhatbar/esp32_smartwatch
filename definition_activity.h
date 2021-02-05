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

//conditions for simplicity
#define facingUp (AcZ < -15000)     //condition to check if watch face is facing up

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int steptimer, stepDuration, movingTimer, movingSteps;            //timer is overall, duration is millis - timer
bool isstep, lastisstep, isrunstep, lastisrunstep;

bool isMoving, lastisMoving;  //has started calc for a step

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
