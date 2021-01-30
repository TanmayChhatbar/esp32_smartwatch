//set correct number of screens
#if defined(bluetoothLogging) and defined(DebuggingScreens)
#define no_screens (basic_screens + 3)
#elif defined(DebuggingScreens)
#define no_screens (basic_screens + 2)
#elif defined(bluetoothLogging)
#define no_screens (basic_screens + 1)
#else no_screens basic_screens
#endif

//set correct number *to* each screen 
#define p1 1
#define p2 (p1 + 1)
#define p3 (p2 + 1)
#ifdef bluetoothLogging
#define p4 (p3 + 1)
#else
#define p4 p3
#endif

#ifdef DebuggingScreens
#define p5 (p4 + 1)
#define p6 (p5 + 1)
#else
#define p5 p4
#define p6 p5
#endif
