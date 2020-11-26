//  Sensor tester
//
//  Chris Herzog - Joliet Cyborgs #4241

#include <LiquidCrystal.h>

#include "defines.h"
#include "buttons.h"
#include "menu.h"


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


extern MENU mainMenu;       // needed so we can do forward referencing due to how the menus intertwine

MENU analogScaledMenu = {
  " Scaled Range:  ", ENABLE_UP,  { { "   Set Lower >  ", AdjustScale, (void *)ADJUST_LOWER_SCALE }, 
                                    { "   < Execute >  ", ReadA2d,     (void *)SENSOR_ANALOG_SCALED },
                                    { "   < Set Upper  ", AdjustScale, (void *)ADJUST_UPPER_SCALE },
                                    { NULL, NULL, NULL } }
};

MENU analogMenu = {
  "Analog Display: ", ENABLE_UP,  { { "    ABSOLUTE >  ", ReadA2d, (void *)SENSOR_ANALOG_ABS }, 
                                    { " < PERCENTAGE > ", ReadA2d, (void *)SENSOR_ANALOG_PERCENT },
                                    { "   < SCALED     ", ExecuteMenu, &analogScaledMenu },
                                    { NULL, NULL, NULL } }
};

MENU digitalPulseMenu = {
  " Counting Edge: ", ENABLE_UP,   { { "   NEGATIVE >   ", ReadDigitalCount, (void *)DIG_READ_NEG_PULSES }, 
                                     { "   <  POSITIVE  ", ReadDigitalCount, (void *)DIG_READ_POS_PULSES },
                                     { NULL, NULL, NULL } }
};

MENU digitalInMenu = {
  " Digital Read:  ", ENABLE_UP,  { { "     STATE >    ", ReadDigitalState, (void *) NULL }, 
                                    { "  < PULSE COUNT ", ExecuteMenu, &digitalPulseMenu },
                                    { NULL, NULL, NULL } }
};

MENU digitalOutMenu = {
  " Digital Write: ", ENABLE_UP,  { { "      HIGH >    ", WriteDigital, (void *) 1 }, 
                                    { "    <  LOW      ", WriteDigital, (void *) 0 },
                                    { NULL, NULL, NULL } }
};

MENU digitalMenu = {
  "  Sensor Type:  ", ENABLE_UP,  { { "      READ >    ", ExecuteMenu, &digitalInMenu }, 
                                    { "   <  WRITE     ", ExecuteMenu, &digitalOutMenu },
                                    { NULL, NULL, NULL } }
};

MENU mainMenu = {
  "  Sensor Type:  ", DISABLE_UP, { { "     ANALOG >   ", ExecuteMenu, &analogMenu }, 
                                    { "   < DIGITAL    ", ExecuteMenu, &digitalMenu },
                                    { NULL, NULL, NULL} }
};

// custom characters

byte upArrowChar[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000,
};

byte downArrowChar[8] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
};

void setup() {

  Serial.begin(SERIAL_SPEED);   // in case we want it for debugging...

  lcd.begin(16, 2);
  lcd.setCursor(1,0);           // display the splash screen
  lcd.print("SENSOR TESTER");
  lcd.setCursor(1, 1);
  lcd.print("Joliet Cyborgs");

  // set custom characters
  lcd.createChar(UP_ARROW_CUSTOM_CHAR, upArrowChar);
  lcd.createChar(DOWN_ARROW_CUSTOM_CHAR, downArrowChar);

  delay(SPLASH_SCREEN_DELAY);
}


//  we cheat and use the button that resets the Arduino as the "HOME" button - pressing it returns
//  us to the main loop

void loop() {

  ExecuteMenu(&mainMenu);
  
/*  
  int button;

  lcd.clear();        // start clean
  
  lcd.setCursor(0, 0);

  lcd.print("  Sensor type:");
  
  while (1) {

  lcd.setCursor(5, 1);
  
  button = ReadLcdButtons();
  
  switch (button) {

    case BTN_RIGHT:
      lcd.print( "RIGHT ");
      break;
     
    case BTN_LEFT:
      lcd.print(" LEFT  ");
      break;
    
    case BTN_UP:
      lcd.print("  UP   ");
      break;

    case BTN_DOWN:
     lcd.print(" DOWN   ");
     break;
     
    case BTN_SELECT:
      lcd.print("SELECT ");
      break;
     
    case BTN_NONE:
      lcd.print(" NONE  ");
      break;
    }
  }
  */
}
