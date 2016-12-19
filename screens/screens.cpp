#include <stdio.h>
#include <unistd.h>

#include "screens.h"
#include "../touchinput.h"
#include "../cli.h"

using namespace std;

int ScreenHandler::currentScreen = 0;

int counter = 0;

void ScreenHandler::updateScreen(WINDOW * maintty) {
  vector<int> touchEvents = TouchInput::getTouchInput();

  if (currentScreen == 0) {
    ScanScreen::maintty = maintty;
    ScanScreen::updateWindow(touchEvents);

    //Some debug stuff
    // /* 
    // string tmp = CLI::convertInt(touchEvents[0]);
    // tmp += ", ";
    // tmp += CLI::convertInt(touchEvents[1]);
    // tmp += ", ";
    // tmp += CLI::convertInt(touchEvents[2]);
    // tmp += ", ";
    // counter++;
    // tmp += CLI::convertInt(counter);
    // mvaddstr(2, 32, (string("X,Y,U,C: ") + string(tmp)).c_str());
    // tmp = TouchInput::touchDevice;
    // mvaddstr(4, 32, (string("T: ") + string(tmp)).c_str());
    // refresh();
    // */
    
  } else if (currentScreen == 1) {
    CellScreen::maintty = maintty;
    CellScreen::updateWindow(touchEvents);
  }

  usleep(1000000); // Refresh rate
}

void ScreenHandler::changeScreen(int newScreen) {
  ScreenHandler::currentScreen = newScreen;
}
