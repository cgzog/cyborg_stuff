// menu.h		menu constants and definitions for Cyborg sensor tester


void  ExecuteMenu(void *);


void  ReadA2d(void *);
void  AdjustScale(void *);

void  WriteDigital(void *);

void  ReadDigitalState(void *);
void  ReadDigitalCount(void *);


#define DISABLE_UP        0
#define ENABLE_UP         1


typedef struct {
  const char  *text;              // text to display - pad with spaces at the start and enough
                                  // at end to clear other selections
                                  // NULL indicates ends of selections
  void  (*func)(void *);          // call this function if selected - could be any function
  void  *param;                   // parameter to pass to function
} MENU_SELECTION;

typedef struct {
  const char      *text;          // shown on the top line - pad with spaces at the start
                                  // NULL indicates end of menus
  const char      enableUp;       // UP button returns to previous menu
  MENU_SELECTION  selections[];   // 
} MENU;
