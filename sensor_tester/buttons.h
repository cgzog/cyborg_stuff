// buttons.h		button handler for Cyborg sensor tester

extern	int	ReadLcdButtons();


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
