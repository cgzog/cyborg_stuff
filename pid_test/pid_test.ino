/********************************************************
 * PID Proportional on measurement Example
 * Setting the PID to use Proportional on measurement will 
 * make the output move more smoothly when the setpoint 
 * is changed.  In addition, it can eliminate overshoot
 * in certain processes like sous-vides.
 ********************************************************/

#include <PID_v1.h>
#include <Servo.h>

//Define Variables we'll be connecting to
double SetpointX1, InputX1, OutputX1;
double SetpointX2, InputX2, OutputX2;

//Specify the links and initial tuning parameters
PID pidX1(&InputX1, &OutputX1, &SetpointX1,1,0,0,P_ON_E, DIRECT); //P_ON_M specifies that Proportional on Measurement be used
                                                            //P_ON_E (Proportional on Error) is the default behavior
PID pidX2(&InputX2, &OutputX2, &SetpointX2,1,0,0,P_ON_E, DIRECT); //P_ON_M specifies that Proportional on Measurement be used

#define IN_MIN            0
#define IN_MAX            1023


#define ANALOG_PIN        A8

#define LED_PIN           7


#define SERVO_PIN         8
#define SERVO_ANGLE_START 90
#define SERVO_ANGLE_MAX   180
#define SERVO_ANGLE_MIN   0

// an A2D value of less than this from our setpoint lights the LED
#define ADJUSTMENT_TOLERANCE    3

#define CYCLE_DELAY       300             // delay between compute and move cycles (ms)



Servo   controlServo;

int     servoAngle = SERVO_ANGLE_START;




void setup()
{
  Serial.begin(115200);
  
  //initialize the variables we're linked to
  InputX1 = analogRead(ANALOG_PIN);
  SetpointX1 = 500;
  InputX2 = IN_MAX - InputX1;
  SetpointX2 = IN_MAX - SetpointX1;

  //turn the PID on
  pidX1.SetMode(AUTOMATIC);
  pidX2.SetMode(AUTOMATIC);

  pinMode(LED_PIN, OUTPUT);

  controlServo.attach(SERVO_PIN);
  controlServo.write(servoAngle);
}

void loop()
{

#ifdef NEVER
while (1) {
  int i;
  
  for (i = SERVO_ANGLE_MIN ; i < SERVO_ANGLE_MAX ; i++) {
    controlServo.write(i);
    delay(CYCLE_DELAY); 
  }
  
  for (i = SERVO_ANGLE_MAX ; i > SERVO_ANGLE_MIN ; i--) {
    controlServo.write(i);
    delay(CYCLE_DELAY); 
  }
}
#endif  
  
  InputX1 = analogRead(ANALOG_PIN);
  InputX2 = IN_MAX - InputX1;
  
  pidX1.Compute();
  pidX2.Compute();

  if (abs(OutputX1 - OutputX2) < ADJUSTMENT_TOLERANCE) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

//  The "output" from the PID is essentially unit-less as far as I can tell so I take the 
//  difference of the two and divide it by 2 to get the servo angle change - through empirical
//  testing, it seems to minimize overshoot
  servoAngle += (OutputX1 - OutputX2)/2;

  if (servoAngle > SERVO_ANGLE_MAX) {
    servoAngle = SERVO_ANGLE_MAX;
  }
  
  if (servoAngle < SERVO_ANGLE_MIN) {
    servoAngle = SERVO_ANGLE_MIN;
  }
  
  controlServo.write(servoAngle);
  delay(CYCLE_DELAY);
  
  Serial.print("Input: ");
  Serial.print(InputX1);
  Serial.print(" / ");
  Serial.print(InputX2);
  Serial.print("  Set: ");
  Serial.print(SetpointX1);
  Serial.print(" / ");
  Serial.print(SetpointX2);
  Serial.print("  Output: ");
  Serial.print(OutputX1);
  Serial.print(" / ");
  Serial.print(OutputX2 * -1);
  Serial.print("  Total: ");
  Serial.print(OutputX1 - OutputX2);
  Serial.print("  Servo Angle: ");
  Serial.println(servoAngle);
}
