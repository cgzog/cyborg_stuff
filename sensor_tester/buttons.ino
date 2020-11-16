// buttons.ino			button handler for Cyborg sensor tester


// this table maps analog reads from the button matrix into codes corresponding to the button pressed

static BUTTON_MATRIX ButtonMatrix[] = {
  { 0,                                 BTN_A2D_RIGHT + BUTTON_TOLERANCE,  BTN_RIGHT },    // can't go below 0
  { BTN_A2D_UP - BUTTON_TOLERANCE,     BTN_A2D_UP + BUTTON_TOLERANCE,     BTN_UP },
  { BTN_A2D_DOWN - BUTTON_TOLERANCE,   BTN_A2D_DOWN + BUTTON_TOLERANCE,   BTN_DOWN },
  { BTN_A2D_LEFT - BUTTON_TOLERANCE,   BTN_A2D_LEFT + BUTTON_TOLERANCE,   BTN_LEFT },
  { BTN_A2D_SELECT - BUTTON_TOLERANCE, BTN_A2D_SELECT + BUTTON_TOLERANCE, BTN_SELECT }
};


// ReadLcdButtons
//
//  to read the LCD buttons using only a single pin on the Arduino, the LCD shield uses
//  an analog input to the Arduino which corresponds to the button being pressed
//
//  on the initial test board, here are the values read from each button press:
//
//    0     right
//    100   up
//    256   down
//    410   left
//    640   select
//
//  these values might not be exact from board to board so we shouldn't count on them being some exact
//  value and instead allow some variance in the values.  we do this by allowing values to vary by
//  up to SENSOR_TOLERANCE (if we're expecting 100, we allow the value to be up to 100 + SENSOR_TOLERANCE).
//
//  by checking starting at the low end through a series of if() statements, we can step through the values
//  one at a time and return the first one that sits in the range of a valid button

int ReadLcdButtons() {
  int  adcValue, i;
 
  adcValue = analogRead(0);      // read the potential key value

  // search the button table for a corresponding button

  for (i = 0 ; i < (sizeof(ButtonMatrix) / sizeof(ButtonMatrix[0])) ; i++) {
    if (adcValue >= ButtonMatrix[i].buttonLow && adcValue <= ButtonMatrix[i].buttonHigh) {
        delay(BUTTON_DEBOUNCE);
        return ButtonMatrix[i].buttonReturn;
    }
  }

  delay(BUTTON_DEBOUNCE);
  return BTN_NONE;              // not within any button ranges
}
