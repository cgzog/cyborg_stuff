// tower_light_test
//
// check the driver for the tower light and make sure it works as expected

//#define DEBUG             // uncomment this line to enable debug messages to come out the serial port


// set which pin controls which light

#define RED_PIN       8
#define YELLOW_PIN    9
#define GREEN_PIN     10

#define ON_BOARD_LED  13    // this is an LED that is on every arduino board - we'll use it to show that the programs is running


// wait this number of milliseconds between each step in the sequence

#define DELAY_PERIOD  300


// define a bit for each light so we can create an array with our light sequence

#define RED_LIGHT     1     // each light has it's own bit
#define YELLOW_LIGHT  2     // if no bits are set, all light go off
#define GREEN_LIGHT   4


// here is the actual sequence

int Sequence[] = {
  GREEN_LIGHT,      // start lighting the stack from the bottom 
  GREEN_LIGHT | YELLOW_LIGHT,
  GREEN_LIGHT | YELLOW_LIGHT | RED_LIGHT,
  GREEN_LIGHT | YELLOW_LIGHT,
  GREEN_LIGHT,
  0,                // give us a double period with no lights on
  0,
  RED_LIGHT,        // bounce the lights up and down a few times
  YELLOW_LIGHT,
  GREEN_LIGHT,
  YELLOW_LIGHT,
  RED_LIGHT,
  YELLOW_LIGHT,
  GREEN_LIGHT,
  YELLOW_LIGHT,
  RED_LIGHT,
  YELLOW_LIGHT,
  GREEN_LIGHT,
  YELLOW_LIGHT,
  RED_LIGHT,
  0,
  0               // another double delay before repeating the whole sequence again
};


//  SetLights
//
//  goes through the bit pattern of the light value passed and sets the respective pins to reflect the state of each light
//
//  fast and simple

void SetLights(int lights) {

int red, yellow, green;

#ifdef DEBUG
  Serial.print("SetLights(");     // disply what we're setting to the serial port
  Serial.print(lights);
  Serial.println(")");
#endif

  digitalWrite(RED_PIN, lights & RED_LIGHT);
  digitalWrite(YELLOW_PIN, lights & YELLOW_LIGHT);
  digitalWrite(GREEN_PIN, lights & GREEN_LIGHT);
}


// this gets called automatically by the arduino code but only once

void setup() {
  
  // setup the pins we need to drive the light as outputs
 
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(ON_BOARD_LED, OUTPUT);

  Serial.begin(115200);

#ifdef DEBUG
  Serial.println("Finished setup()");
#endif
}


// this gets called automatically by the arduino code and keeps getting called if we exit this function

void loop() {

int i;

#ifdef DEBUG
  Serial.print("Starting a sequence ...   ");
#endif

  digitalWrite(ON_BOARD_LED, HIGH);   // turn it on when we start the loop
  
  for (i = 0 ; i < (sizeof(Sequence) / sizeof(Sequence[0])) ; i++) {

    SetLights(Sequence[i]);

    delay(DELAY_PERIOD);
  }

#ifdef DEBUG
  Serial.println("... sequence completed");
#endif

  digitalWrite(ON_BOARD_LED, LOW);   // turn it off when are done the loop
  
  delay(1000);                // give us a chance to see it actually go off before starting the next loop
}
