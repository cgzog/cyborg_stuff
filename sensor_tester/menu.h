// menu.h		menu constants and definitions for Cyborg sensor tester


void ExecuteMenu(void *);


typedef struct {
  char  *text;                    // text to display - pad with spaces at the start and enough
                                  // at end to clear other selections
                                  // NULL indicates ends of selections
  void  (*func)(void *);          // call this function if selected - could be any function
  void  *param;                   // parameter to pass to function
} MENU_SELECTION;

typedef struct {
  char            *text;      // shown on the top line - pad with spaces at the start
                                  // NULL indicates end of menus
  MENU_SELECTION  selections[];   // 
} MENU;
