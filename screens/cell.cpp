#include <stdio.h>
#include <curses.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include <pthread.h>

#include "../wificell.h"
#include "../wifilist.h"
#include "../cli.h"
#include "../uiobj.h"
#include "../touchinput.h" // Todo: Update touchInput to byref
#include "cell.h"
#include "screens.h"
#include "../keyboardwatch.h"

vector<UIObject> CellScreen::uiObjects;

WINDOW * CellScreen::maintty;
string CellScreen::cellMAC = "";
WifiCell CellScreen::scanningCellLeft;
WifiCell CellScreen::scanningCellRight;
int CellScreen::scanState = -1;
string CellScreen::leftScanner = "wlan0";
string CellScreen::rightScanner = "wlan0";

void CellScreen::setupKeyboardEvents() {
  KeyboardWatch::clearKeys();
  KeyboardWatch::addKey(8, &CellScreen::btnBack); // Backspace
  KeyboardWatch::addKey(KEY_LEFT, &CellScreen::btnBack);
  KeyboardWatch::addKey(27, &CellScreen::btnExit); // Escape
}

void * CellScreen::scanArea(void *threadID) {
  if (CellScreen::scanState == -1 || CellScreen::scanState == 0) {
    vector<WifiCell> cellList;
    CellScreen::scanState = 1;
    WifiList * wifiList = new WifiList();

    wifiList->wifiScan(leftScanner);
    cellList = wifiList->getWifiList();

    bool found = false;
    for(vector<WifiCell>::size_type i = 0; i < cellList.size(); i++) {
      if (cellList[i].getMAC() == CellScreen::cellMAC) {
        CellScreen::scanningCellLeft = cellList[i];
        found = true;
        break;
      }
    }

    if (!found) {
      CellScreen::scanningCellLeft.setSignalLevel("-256");
      CellScreen::scanningCellLeft.setLinkQuality(string("0/" + CLI::convertInt(CellScreen::scanningCellLeft.getLinkQualityMax())));
    }

    wifiList->wifiScan(rightScanner);
    cellList = wifiList->getWifiList();

    found = false;
    for(vector<WifiCell>::size_type i = 0; i < cellList.size(); i++) {
      if (cellList[i].getMAC() == CellScreen::cellMAC) {
        CellScreen::scanningCellRight = cellList[i];
        found = true;
        break;
      }
    }

    if (!found) {
      CellScreen::scanningCellRight.setSignalLevel("-256");
      CellScreen::scanningCellRight.setLinkQuality(string("0/" + CLI::convertInt(CellScreen::scanningCellRight.getLinkQualityMax())));
    }

    delete wifiList;
    CellScreen::scanState = 0;
  }

}

void CellScreen::updateWindow(vector<int> touchEvents) {
    clear();
    CellScreen::drawBorder();

    CellScreen::checkTouchEvents(touchEvents);
    CellScreen::generateUIObjects();

    if (CellScreen::scanState == 0) {
      pthread_t scanThread;
      int updateScans = pthread_create(&scanThread, NULL, CellScreen::scanArea, (void *)0);
    } else if (CellScreen::scanState <= -1) {
      pthread_t scanThread;
      int updateScans = pthread_create(&scanThread, NULL, CellScreen::scanArea, (void *)0);
      usleep(3000000);
      return ;
    }

    int levelLeft = (int)(( (float)CellScreen::scanningCellLeft.getLinkQualityLower() / (float)CellScreen::scanningCellLeft.getLinkQualityMax() ) * 35);
    int levelRight = (int)(( (float)CellScreen::scanningCellRight.getLinkQualityLower() / (float)CellScreen::scanningCellRight.getLinkQualityMax() ) * 35);

    mvaddstr(1, 40, "dBm");

    mvaddstr(2, 37, CellScreen::scanningCellLeft.getSignalLevel().c_str());
    CellScreen::drawGraph(37, 3, 35, levelLeft);

    mvaddstr(2, 42, CellScreen::scanningCellRight.getSignalLevel().c_str());
    CellScreen::drawGraph(42, 3, 35, levelRight);

    mvaddstr(3, 2,"Wifi Name :        ");
    mvaddstr(4, 2,"MAC Address :      ");
    mvaddstr(5, 2,"Scanning Devices : ");
    mvaddstr(6, 2,"Logging to File :  ");

    attron(COLOR_PAIR(2));
    mvaddstr(3, 16, CellScreen::scanningCellLeft.getESSID().c_str());
    mvaddstr(4, 16, CellScreen::scanningCellLeft.getMAC().c_str());
    mvaddstr(5, 21, string(CellScreen::leftScanner + " " + CellScreen::leftScanner).c_str());
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(9));
    mvaddstr(7, 4, "None");
    attroff(COLOR_PAIR(9));

    // mvaddstr(20, 5, CellScreen::cellMAC.c_str());
    // mvaddstr(21, 5, CellScreen::scanningCell.getMAC().c_str());
    // mvaddstr(22, 5, CellScreen::scanningCell.getSignalLevel().c_str());
    // mvaddstr(23, 5, CellScreen::scanningCell.getLinkQuality().c_str());
    // mvaddstr(24, 5, CLI::convertInt(CellScreen::scanningCell.getLinkQualityLower()).c_str());
    // mvaddstr(25, 5, CLI::convertInt(CellScreen::scanningCell.getLinkQualityMax()).c_str());
    // mvaddstr(26, 5, CLI::convertInt(level).c_str());

    // drawControls();
    CellScreen::drawExit();
    CellScreen::drawBack();
    refresh();
}

void CellScreen::drawGraph(int x, int y, int s, int d) {

  mvaddstr(y, x, " -- ");
  int j = 0;
  for (int i = s; i > 0; i--, j++) {
    mvaddstr(y + i, x, "|  |");

    if (j < d) {
      attron(COLOR_PAIR(7));
      mvaddstr(y + i, x + 1, "  ");
      attroff(COLOR_PAIR(7));
    }
  }
  mvaddstr(y + s, x, " -- ");
}

void CellScreen::generateUIObjects() {
  // Possibly needed to avoid memory leak.
  //for(vector<UIObject>::size_type i = 0; i != CellScreen::uiObjects.size(); i++) {
  //  delete CellScreen::uiObjects[i];
  //}

  CellScreen::uiObjects.clear();

  CellScreen::uiObjects.push_back(UIObject("btnExit", 221, 3386, 731, 3722, &CellScreen::btnExit));
  CellScreen::uiObjects.push_back(UIObject("btnBack", 3012, 3386, 3799, 3722, &CellScreen::btnBack));
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

void CellScreen::drawBack() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    vector<string> backButton;
    backButton.push_back(" -------- ");
    backButton.push_back("|        |");
    backButton.push_back("|  XXXX  |");
    backButton.push_back("|        |");
    backButton.push_back(" -------- ");

    for(unsigned int i = 0; i < backButton.size(); i++ ) {
      attron(COLOR_PAIR(5));
      mvaddstr((w.ws_row - backButton.size() + i) - 1, 1, backButton[i].c_str());
      attroff(COLOR_PAIR(5));
    }

    int backPosX = 4;
    int backPosY = (w.ws_row - backButton.size()) + (floor(backButton.size() / 2) - 1);
    attron(COLOR_PAIR(5));
    mvaddstr(backPosY, backPosX, "BACK");
    attroff(COLOR_PAIR(5));
}

void CellScreen::btnBack() {
  ScreenHandler::changeScreen(0);
}

