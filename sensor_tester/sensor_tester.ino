//  Sensor tester
//
//  Chris Herzog - Joliet Cyborgs #4241

#include <LiquidCrystal.h>

#include "defines.h"
#include "buttons.h"
#include "menu.h"


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


extern MENU MainMenu;       // needed so we can do forward referencing due to how the menus intertwine

MENU AnalogMenu = {
  "Analog Display:", { { "    ABSOLUTE >   ", ExecuteMenu, &MainMenu }, 
                       { " < PERCENTAGE >  ", ExecuteMenu, &MainMenu },
                       { "   < SCALED      ", ExecuteMenu, &MainMenu },
                       { NULL, NULL, NULL} }
};

MENU DigitalMenu = {
  "  Sensor Type:", { { "      READ >      ", ExecuteMenu, &MainMenu }, 
                      { "   <  WRITE       ", ExecuteMenu, &MainMenu },
                      { NULL, NULL, NULL} }
};

MENU MainMenu = {
  "  Sensor Type:", { { "     ANALOG >     ", ExecuteMenu, &AnalogMenu }, 
                      { "   < DIGITAL      ", ExecuteMenu, &DigitalMenu },
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

  ExecuteMenu(&MainMenu);
  
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
