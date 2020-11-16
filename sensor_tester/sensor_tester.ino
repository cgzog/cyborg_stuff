//  Sensor tester
//
//  Chris Herzog - Joliet Cyborgs #4241

#include <LiquidCrystal.h>


#define SPLASH_SCREEN_DELAY   2000        // in milliseconds


void ExecuteMenu(void *);


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// constants returned from button presses

#define BTN_RIGHT  0
#define BTN_UP     1
#define BTN_DOWN   2
#define BTN_LEFT   3
#define BTN_SELECT 4
#define BTN_NONE   999


//  button analog values (determined through reading them from an actual board
//
//  the board used was a generic "LCD Keypad Shield" from eBay - they may need
//  to be adjusted pre board
//
//  we allow button values to OK so long as they are within our BUTTON_TOLERANCE value
//
//  the BUTTON_TOLERANCE value needs to be small enough so that button values don't overlap;
//  this would make button presses ambigous - programmers hate things like that

#define BTN_A2D_RIGHT     0
#define BTN_A2D_UP        100
#define BTN_A2D_DOWN      256
#define BTN_A2D_LEFT      410
#define BTN_A2D_SELECT    640

#define BUTTON_TOLERANCE  40

typedef struct  {
  int buttonLow;        // the lowest A2D value we'll accept for this button
  int buttonHigh;       // the highest value
  int buttonReturn;     // button this range corresponds to
} BUTTON_MATRIX;

BUTTON_MATRIX ButtonMatrix[] = {
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
//  up to 50 (if we're expecting 100, we allow the value to be up to 150).
//
//  by checking starting at the low end through a series of if() statements, we can step through the values
//  one at a time and return the first one that passes our criteria

int ReadLcdButtons() {
  int  adcValue, i;
 
  adcValue = analogRead(0);      // read the potential key value

  // search the button table for a corresponding button

  for (i = 0 ; i < (sizeof(ButtonMatrix) / sizeof(ButtonMatrix[0])) ; i++) {
    if (adcValue >= ButtonMatrix[i].buttonLow && adcValue <= ButtonMatrix[i].buttonHigh) {
        return ButtonMatrix[i].buttonReturn;
    }
  }

  return BTN_NONE;  // not within any button ranges
}




typedef struct {
  char  *selection;               // text to display - pad with spaces at the start
                                  // NULL indicates ends of selections
  int   button;                   // this button controls the function to be called
  void  (*func)(void *);          // call this function if selected - could be any function
} MENU_SELECTION;

typedef struct {
  char            *menuText;      // shown on the top line - pad with spaces at the start
                                  // NULL indicates end of menus
  MENU_SELECTION  selections[];   // 
} MENU;



//  ExecuteMenu
//
//  run that menu and call other routines based on what gets selected

void ExecuteMenu(void *menu) {
int button;

    lcd.clear();
    
    lcd.setCursor(0, 0);                        // start at the first character of the first line

    lcd.print(((MENU *)menu)[0].menuText);                // display the top line

    lcd.setCursor(0, 1);                        // position at the start of the second line

    lcd.print(((MENU *)menu)[0].selections[0].selection); // display the first selection

    while ((button = ReadLcdButtons()) == BTN_NONE) {
      // spin until there is a button
    }

    
}









void setup() {
  
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
}
