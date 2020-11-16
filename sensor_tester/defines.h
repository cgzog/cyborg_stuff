//	defines.h for the sensor tester
//
// common defines reside here

#define SPLASH_SCREEN_DELAY   2000        // in milliseconds


#define SERIAL_SPEED          115200


// constants returned from button presses

#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
#define BTN_NONE   999

#define BUTTON_DEBOUNCE       75          // wait this many ms between button reads


#define SENSOR_ANALOG         0           // sensor type
#define SENSOR_DIGITAL        1

#define SENSOR_ANALOG_ABS     0           // type within that sensor
#define SENSOR_ANALOG_PERC    1
#define SENSOR_ANALOG_SCALED  2

#define SENSOR_DIGITAL_READ   0
#define SENSOR_DIGITAL_WRITE  1


#define DEBUG                           // enable to compile debugging code
