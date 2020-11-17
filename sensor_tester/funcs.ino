//	funcs.ino		do the actual functions off of the menu

#define L_JUSTIFY       0             // for displaying numeric values
#define R_JUSTIFY       1


// used only for analog scaled display
static  int lowScale  = DEF_LOW_SCALE;
static  int highScale = DEF_HIGH_SCALE;



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
                            break;

      case SENSOR_ANALOG_PERCENT:
                            lcd.setCursor(6, 1);
                            formattedLcd((int)(analogVal * 100 / ANALOG_FULL_SCALE), 3, R_JUSTIFY); // all integer math this way!
                                                                                                    // keeps the code *way* smaller
                            lcd.print("%");
                            break;

      case SENSOR_ANALOG_SCALED:
                            lcd.setCursor(0, 1);
                            formattedLcd(lowScale, 5, L_JUSTIFY);
                            lcd.print(" ");
                            formattedLcd(map((int)analogVal, 0, ANALOG_FULL_SCALE, lowScale, highScale), 5, R_JUSTIFY);
                            formattedLcd(highScale, 5, R_JUSTIFY);
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
