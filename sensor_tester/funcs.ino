//	funcs.ino		do the actual functions off of the menu

#define L_JUSTIFY       0             // for displaying numeric values
#define R_JUSTIFY       1


// used only for analog scaled display
static  int lowScale  = DEF_LOW_SCALE;
static  int highScale = DEF_HIGH_SCALE;



// SerialOutput
//
// log output - timestamp (milliseconds from start) <SPACE> tag identifying type of measurement <SPACE> value

static void serialLogger(const char * string, int value) {
  long now;

  now = millis();

  Serial.print(now);
  Serial.print(" ");
  Serial.print(string);
  Serial.print(" ");
  Serial.println(value);
}



static void formattedLcd(int val, int length, int justify) {

int valLength, valAbs, i;

  if (val < 0) {
    valAbs = val * -1;
  } else {
    valAbs = val;
  }

  // how long is the number?

  if (valAbs < 10)
    valLength = 1;
  else if (valAbs < 100)
     valLength = 2; 
  else if (valAbs < 1000)
    valLength = 3;
  else if (valAbs < 10000)
    valLength = 4;
  else
    valLength = 5;

  if (val < 0) {
    valLength++;  // include the negative sign
  }

  if (justify == L_JUSTIFY) {     // number then spaces
    lcd.print(val);
  }

  for (i = 0 ; i < length - valLength ; i++) {
    lcd.print(" ");               // space end up after for L_JUSTIFY and before for R_JUSTIFY
  }

  if (justify == R_JUSTIFY) {
    lcd.print(val);
  }
}



void ReadA2d(void *param) {

unsigned long analogVal;

  lcd.clear();
  lcd.print(" Analog ");
  
  switch ((int)param) {
    case SENSOR_ANALOG_ABS:
                            lcd.print("(ABS):");
                            break;

    case SENSOR_ANALOG_PERCENT:
                            lcd.print("(Perc):");
                            break;

    case SENSOR_ANALOG_SCALED:
                            lcd.print("(Scaled):");
                            break;
  }

  lcd.setCursor(0, 1);                          // position at the start of the second line
  lcd.print("               ");                 // clear the second line
    
  while (1) {

    lcd.setCursor(7, 1);                        // position partway into second line
    
    analogVal = analogRead(ANALOG_READ_PORT);

    switch ((int)param) {

      case SENSOR_ANALOG_ABS:
                            lcd.setCursor(6, 1);
                            formattedLcd(analogVal, 4, R_JUSTIFY);
                            serialLogger("A-ABS", analogVal);
                            break;

      case SENSOR_ANALOG_PERCENT:
                            int percent;
                            
                            lcd.setCursor(6, 1);

                            percent = analogVal * 100 / ANALOG_FULL_SCALE;

                            lcd.print(percent);
                            lcd.print("%  ");
                            
                            serialLogger("A-%", percent);
                            break;

      case SENSOR_ANALOG_SCALED:
                            int scaled;

                            scaled = map((int)analogVal, 0, ANALOG_FULL_SCALE, lowScale, highScale);
                            lcd.setCursor(0, 1);
                            formattedLcd(lowScale, 5, L_JUSTIFY);
                            lcd.print(" ");
                            
                            formattedLcd(scaled, 5, R_JUSTIFY);
                            formattedLcd(highScale, 5, R_JUSTIFY);
                            
                            serialLogger("A-SCL", scaled);
                            break;
    }

    lcd.print("    ");                          // cleanup anything at the end of the line

    delay(DATA_READ_DELAY);
  }
}



void AdjustScale(void *param) {

int *scalePtr, button;

  lcd.clear();
  lcd.setCursor(0, 0);                            // start at the first character of the first line

  
  if ((int)param == ADJUST_LOWER_SCALE) {
    lcd.print("Set lower scale:");
    scalePtr = &lowScale;                         // pointer to the scale value makes code simpler
  } else {
    lcd.print("Set upper scale:");
    scalePtr = &highScale;
  }

  while (1) {                                     // loop until we finish

    lcd.setCursor(5, 1);                          // position for the second line
    lcd.print(*scalePtr);                         // show the current value
    lcd.print("    ");                            // cleanup any leftovers
    
    if ((button = ReadLcdButtons()) != BTN_NONE) {
      
      // has the current item been selected?
      if (button == BTN_SELECT) {
//        ExecuteMenu(&analogScaledMenu);
      }

      // otherwise see if we have something to change in regards to the displayed menu item
      switch (button) {

        case BTN_UP:
                        *scalePtr += 1;
                        break;

        case BTN_DOWN:
                        *scalePtr -= 1;
                        break;

        case BTN_LEFT:
                        *scalePtr -= 10;
                        break;

       case BTN_RIGHT:
                        *scalePtr += 10;
                        break;
      }

      if (*scalePtr < SCALE_LOW_LIMIT) {
        *scalePtr = SCALE_LOW_LIMIT;
      } else {
        if (*scalePtr > SCALE_HIGH_LIMIT) {
          *scalePtr = SCALE_HIGH_LIMIT;
        }
      }
    }
  }
}


//  WriteDigital
//
//  process the value and set the digital port appropriately

void  WriteDigital(void *param) {

  pinMode(DIGITAL_PORT, OUTPUT);

  if ((int)param) {
    digitalWrite(DIGITAL_PORT, HIGH);
    serialLogger("DW", 1);
  } else {
    digitalWrite(DIGITAL_PORT, LOW);
    serialLogger("DW", 0);
  }
}


//  ReadDigitalState
//
//  display the state - high or low - of the digital input

void ReadDigitalState(void *param) {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Digital Input:");

  pinMode(DIGITAL_PORT, INPUT);

  while (1) {
    lcd.setCursor(4, 1);                          // position for the second line

    if (digitalRead(DIGITAL_PORT) == HIGH) {
      lcd.print("^ HIGH ^");
      serialLogger("DR", 1);
    } else {
      lcd.print("v LOW v ");
      serialLogger("DR", 0);
    }
  }
}

volatile  unsigned int pulseCount = 0;


//  pulseCount
//
//  this is called from an interrupt and counts a pulse each time it gets called

void pulseCounter() {
  pulseCount++;
}



//  ReadDigitalCount
//
//  count the pulses - either neg or pos as per the incoming parameter
//
//  count is maintained by an ISR routine and we display it as well as we can in the event of it increasing
//
//  since the pulses could be coming in very quickly and updated in an ISR, our count shoudl be accurate but not
//  every count may be displayed (the count could jump)

void ReadDigitalCount(void *param) {

unsigned  int prevPulseCount = 0;
const char    *pulseType;
byte          customChar;

  pulseType = (const char *)((int)param == DIG_READ_NEG_PULSES ? "PN" : "PH");

  serialLogger(pulseType, 0);     // we start hardcoded at 0 just before we zero the count to give us a starting point
  
  pulseCount = 0;                 // pulseCount is a volatile
  prevPulseCount = pulseCount;

   if ((int)param == DIG_READ_NEG_PULSES) {
    customChar = DOWN_ARROW_CUSTOM_CHAR;
  } else {
    customChar = UP_ARROW_CUSTOM_CHAR;
  }
  
  pinMode(DIGITAL_PORT, INPUT);

  attachInterrupt(digitalPinToInterrupt(DIGITAL_PORT), pulseCounter, (int)param == DIG_READ_NEG_PULSES ? FALLING : RISING);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(customChar);
  lcd.print( " Pulse Count ");
  lcd.write(customChar);
  
  lcd.setCursor(3, 1);
  lcd.print("Count: ");

  while (1) {
    if (prevPulseCount != pulseCount) {             // update the display and serial output only on change
      lcd.setCursor(10, 1);                         // position for the second line
      lcd.print(pulseCount);                        // count gets updated in an ISR

      serialLogger(pulseType, pulseCount);
    }
  }
}
