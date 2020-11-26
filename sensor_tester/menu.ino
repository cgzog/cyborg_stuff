// menu.ino			menu handlers for Cyborg sensor tester



//  ExecuteMenu
//
//  run that menu and call other routines based on what gets selected

void ExecuteMenu(void *menu) {
  
int button, menuSelection;

  lcd.clear();
  lcd.setCursor(0, 0);                            // start at the first character of the first line
  lcd.print(((MENU *)menu)->text);                // display the top line

  menuSelection = 0;                              // start with the first menu selection

  while (1) {                                     // loop until we find something

    lcd.setCursor(0, 1);                          // position at the start of the second line
    lcd.print(((MENU *)menu)->selections[menuSelection].text);  // display the current selection

    if ((button = ReadLcdButtons()) != BTN_NONE) {
      
      // has the current item been selected?
      if (button == BTN_SELECT) {
        ((MENU *)menu)->selections[menuSelection].func(((MENU *)menu)->selections[menuSelection].param);
        
        continue;                                 // back to the while()...
      }

#ifdef  DEBUG
      Serial.print("EM: key ");
      Serial.println(menuSelection);
      Serial.print("  ");
      Serial.println(menuSelection);
#endif
      // otherwise see if we have something to change in regards to the displayed menu item
      switch (button) {

        case BTN_LEFT:
                        // check to see if we'd go before the first item
                        if (menuSelection > 0) {
                          menuSelection--;
                        }
                        break;

        case BTN_RIGHT:
                        //check to see if we'd go past the last item
                        if (((MENU *)menu)->selections[menuSelection + 1].text != NULL) {
                          menuSelection++;
                        }
                        break;

        case BTN_UP:
                        // check to see if the UP button takes us back
                        if (((MENU *)menu)->enableUp == ENABLE_UP) {
                          return;
                        }
                        break;
      }
    }
  }
}
