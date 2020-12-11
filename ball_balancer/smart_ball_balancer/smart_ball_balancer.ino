
#include <stdint.h>

#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <PID_v1.h>

#include "Adafruit_STMPE610.h"

// Designed to use the Adafruit STMPE610 smart touch screen controller
//
// Connects using I2C:
//
//  STMPE610 Board          Arduino
//    SDA                     SDA
//    SCL                     SCL
//    Mode                    GND
//    Gnd                     GND
//    Vin                     3.3v

Adafruit_STMPE610 touch = Adafruit_STMPE610();


// servo-related
#define X_SERVO 7
#define Y_SERVO 6

#define SERVO_START_POS     90

Servo   XServo, YServo;

// current servo angles
int     XServoAngle = SERVO_START_POS;
int     YServoAngle = SERVO_START_POS;


//  balance-related

// values for the panel corners
#define PANEL_MIN_X     0
#define PANEL_MAX_X     4000
#define PANEL_MIN_Y     0
#define PANEL_MAX_Y     4000

// start with the setpoint right in the middle of the screen
int  XSetPoint   = (PANEL_MIN_X + PANEL_MAX_X) / 2;
int  YSetPoint   = (PANEL_MIN_Y + PANEL_MAX_Y) / 2;

// constants are different between X and Y because the table is a rectangle and not a
// square so the X error can be larger and would require a bigger correction
#define KP_X          0.005
#define KI_X          0.006
#define KD_X          0.0
#define KP_Y          0.005
#define KI_Y          0.0025
#define KD_Y          0.0

// servo angle ranges for tilting bed
#define X_SERVO_MIN   70
#define X_SERVO_MAX   110
#define X_SERVO_START 90
#define Y_SERVO_MIN   70
#define Y_SERVO_MAX   110
#define Y_SERVO_START 90

//  what's close enough to the setpoint?
#define TOLERANCE     25


#define PID_SAMPLE_TIME 100   // ms


class Balance {

  protected:

  int     setPoint;
  double  kP, kI, kD;
  int     minRange, maxRange;
  double  prevAdjustment;
  int     tolerance = TOLERANCE;
  const char  *tag;

  
  public:

  Balance(const char *tagIn, int setPointIn, int minVal, int maxVal, double kPIn, double kIIn, double kDIn) {

    tag = tagIn;
    
    minRange = minVal;
    maxRange = maxVal;

    setPoint = setPointIn;

    prevAdjustment = 0;

    kP = kPIn;
    kI = kIIn;
    kD = kDIn;

    Serial.print("balance->Balance(");
    Serial.print(tag);
    Serial.print(", ");
    Serial.print(setPoint);
    Serial.print(", ");
    Serial.print(minRange);
    Serial.print(", ");
    Serial.print(maxRange);
    Serial.print(", ");
    Serial.print(kP);
    Serial.print(", ");
    Serial.print(kI);
    Serial.print(", ");
    Serial.print(kD);
    Serial.println(")");
  }

  ~Balance() {
    
  }

  void setSetPoint(int setPointIn) {

    setPoint = setPointIn;
  }

  void compute(int current, int *out){

  double tempOut;
  
      if (abs(current - setPoint) < tolerance) {
      prevAdjustment = 0;

      *out = 0;

#ifdef DEBUG
      Serial.print(tag);
      Serial.println(" Z0Z");
#endif
    
      return;
    }

    // we do this as an all floating point calculation so that small errors
    // accumulate and eventually correct out
    tempOut = ((double)current - (double)setPoint) * kP + prevAdjustment * kI;

    prevAdjustment += tempOut;

    // return it as an int but we're keeping the accumulated error as a double
   *out = tempOut;

#ifdef DEBUG
    Serial.print("b->c ");
    Serial.print(current);
    Serial.print(" ");
    Serial.print(*out);
    Serial.print(" ");
    Serial.print(current);
    Serial.print(" ");
    Serial.print(setPoint);
    Serial.print(" ");
    Serial.print(kP);
    Serial.print(" ");
    Serial.print(prevAdjustment);
    Serial.print(" ");
    Serial.print(kI);
    Serial.println("");
#endif
  }
};


Balance *balanceX, *balanceY;


void setup(void) {
  Serial.begin(115200);

  Serial.println("Smart Ball Balancer\n");

  Serial.println("  Initializing touch screen");
  if (! touch.begin(0x41)) {
    Serial.println("STMPE not found!");
    while(1);
  }
  
  Serial.println("  Setting up servos...");
  XServo.attach(X_SERVO);
  YServo.attach(Y_SERVO);
  XServo.write(X_SERVO_START);
  YServo.write(Y_SERVO_START);

  Serial.println("  Setting up balance PID...");
  balanceX = new Balance("X", XSetPoint, PANEL_MIN_X, PANEL_MAX_X, (double)KP_X, KI_X, KD_X);
  balanceY = new Balance("Y", YSetPoint, PANEL_MIN_Y, PANEL_MAX_Y, KP_Y, KI_Y, KD_Y);
  
  Serial.println("READY!\n");
}

void loop(void) {

  uint16_t  x, y;
  uint8_t   z;
  int       outputX, outputY;

  if (touch.touched()) {
    
    // read x & y & z;
    
    while (! touch.bufferEmpty()) {
      
      touch.readData(&x, &y, &z);

      balanceX->compute((int)x, &outputX);
      balanceY->compute((int)y, &outputY);

      Serial.print(x);
      Serial.print(" ");
      Serial.print(y);
      Serial.print(" ");
      Serial.print(outputX);
      Serial.print(" ");
      Serial.println(outputY);

      // the sign depends on how the servo linkages are setup
      XServoAngle = X_SERVO_START - outputX;
      YServoAngle = Y_SERVO_START + outputY;

      if (XServoAngle > X_SERVO_MAX) {
        XServoAngle = X_SERVO_MAX;
      }
      if (XServoAngle < X_SERVO_MIN) {
        XServoAngle = X_SERVO_MIN;
      }
      
      if (YServoAngle > Y_SERVO_MAX) {
        YServoAngle = Y_SERVO_MAX;
      }
      if (YServoAngle < Y_SERVO_MIN) {
        YServoAngle = Y_SERVO_MIN;
      }

      XServo.write(XServoAngle);
      YServo.write(YServoAngle);
    }
  }
  
  // reset all ints, in this example unneeded depending in use
  //
  // this is and the delay is right from the adafruit example program
  touch.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints, in this example unneeded depending in use

  delay(10);
}
