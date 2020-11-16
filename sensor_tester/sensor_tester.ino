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
  " Scaled Range:  ", { { "   Set Lower >   ", ExecuteMenu, &mainMenu }, 
                       { " < Set Upper     ", ExecuteMenu, &mainMenu },
                       { NULL, NULL, NULL} }
};

MENU analogMenu = {
  "Analog Display: ", { { "    ABSOLUTE >   ", ExecuteMenu, &mainMenu }, 
                       { " < PERCENTAGE >  ", ExecuteMenu, &mainMenu },
                       { "   < SCALED      ", ExecuteMenu, &analogScaledMenu },
                       { NULL, NULL, NULL} }
};

MENU digitalPulseMenu = {
  " Counting Edge: ", { { "   NEGATIVE >     ", ExecuteMenu, &mainMenu }, 
                      { "   <  POSITIVE    ", ExecuteMenu, &mainMenu },
                      { NULL, NULL, NULL} }
};

MENU digitalInMenu = {
  " Digital Read:  ", { { "      STATE >      ", ExecuteMenu, &mainMenu }, 
                      { "  < PULSE COUNT    ", ExecuteMenu, &digitalPulseMenu },
                      { NULL, NULL, NULL} }
};

MENU digitalOutMenu = {
  " Digital Write: ", { { "      HIGH >      ", ExecuteMenu, &mainMenu }, 
                      { "    <  LOW        ", ExecuteMenu, &mainMenu },
                      { NULL, NULL, NULL} }
};

MENU digitalMenu = {
  "  Sensor Type:  ", { { "      READ >      ", ExecuteMenu, &digitalInMenu }, 
                      { "   <  WRITE       ", ExecuteMenu, &digitalOutMenu },
                      { NULL, NULL, NULL} }
};

MENU mainMenu = {
  "  Sensor Type:  ", { { "     ANALOG >     ", ExecuteMenu, &analogMenu }, 
                      { "   < DIGITAL      ", ExecuteMenu, &digitalMenu },
                      { NULL, NULL, NULL} }
};


void setup() {

  Serial.begin(SERIAL_SPEED);   // in case we want it for debugging...
  
  lcd.begin(16, 2);
  lcd.setCursor(1,0);           // display the splash screen
  lcd.print("SENSOR TESTER");
  lcd.setCursor(1, 1);
  lcd.print("Joliet Cyborgs");

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
