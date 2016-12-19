#include <stdio.h>
#include <curses.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include "../wificell.h"
#include "../wifilist.h"
#include "../cli.h"
#include "../uiobj.h"
#include "../touchinput.h" // Todo: Update touchInput to byref
#include "cell.h"

vector<UIObject> CellScreen::uiObjects;
WINDOW * CellScreen::maintty;

void CellScreen::updateWindow(vector<int> touchEvents) {
    clear();
    CellScreen::drawBorder();

    CellScreen::checkTouchEvents(touchEvents);
    CellScreen::generateUIObjects();

    // drawControls();
    drawExit();

    refresh();
}

void CellScreen::generateUIObjects() {
  // Possibly needed to avoid memory leak.
  //for(vector<UIObject>::size_type i = 0; i != ScanScreen::uiObjects.size(); i++) {
  //  delete ScanScreen::uiObjects[i];
  //}

  CellScreen::uiObjects.clear();

  CellScreen::uiObjects.push_back(UIObject("btnExit", 221, 3386, 731, 3722, &CellScreen::btnExit));
}

void CellScreen::checkTouchEvents(vector<int> touchEvents) {
  if (touchEvents[2] == 0) {
    return ;
  }

  // Todo: Update touchInput to byref
  TouchInput::eventProcessed();
  touchEvents[2] = 0;

  for(vector<UIObject>::size_type i = 0; i != CellScreen::uiObjects.size(); i++) {
    if (CellScreen::uiObjects.at(i).collisionDetection(touchEvents[0], touchEvents[1])) {
      CellScreen::uiObjects.at(i).triggerClickEvent();
    }
  }
}

void CellScreen::drawExit() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    vector<string> exitButton;
    exitButton.push_back(" -------- ");
    exitButton.push_back("|        |");
    exitButton.push_back("|  XXXX  |");
    exitButton.push_back("|        |");
    exitButton.push_back(" -------- ");

    for(unsigned int i = 0; i < exitButton.size(); i++ ) {
      attron(COLOR_PAIR(3));
      mvaddstr((w.ws_row - exitButton.size() + i) - 1, (w.ws_col - exitButton[0].size()) - 1, exitButton[i].c_str());
      attroff(COLOR_PAIR(3));
    }

    int exitPosX = ((w.ws_col - exitButton[0].size()) - 1) + 3;
    int exitPosY = (w.ws_row - exitButton.size()) + (floor(exitButton.size() / 2) - 1);
    attron(COLOR_PAIR(4));
    mvaddstr(exitPosY, exitPosX, "EXIT");
    attroff(COLOR_PAIR(4));
}

void CellScreen::btnExit() {
  delwin(CellScreen::maintty);
  endwin();
  exit(0);
}

void CellScreen::drawBorder() {
  attron(COLOR_PAIR(1));
  box(CellScreen::maintty, '|', '-');
  attroff(COLOR_PAIR(1));
}


