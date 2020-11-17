//	defines.h for the sensor tester
//
// common defines reside here

#define ANALOG_READ_PORT      A5
#define ANALOG_FULL_SCALE     1023        // max analog read value


#define DEF_HIGH_SCALE        500
#define DEF_LOW_SCALE         0

#define ADJUST_LOWER_SCALE    -1
#define ADJUST_UPPER_SCALE    1

#define SCALE_LOW_LIMIT       -9999
#define SCALE_HIGH_LIMIT      9999


#define DATA_READ_DELAY       200         // ms delay between data reads


#define SPLASH_SCREEN_DELAY   2000        // in milliseconds


#define SERIAL_SPEED          115200


// constants returned from button presses

#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
#define BTN_NONE   999

#define BUTTON_DEBOUNCE       100          // wait this many ms between button reads



#define SENSOR_ANALOG_ABS     0           // sensor type and display mode
#define SENSOR_ANALOG_PERCENT 1
#define SENSOR_ANALOG_SCALED  2

#define SENSOR_DIGITAL_READ   3
#define SENSOR_DIGITAL_WRITE  4


#define DEBUG                           // enable to compile debugging code
