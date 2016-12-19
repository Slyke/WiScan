#include <stdio.h>
#include <curses.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <cstdlib>

#include <pthread.h>

#include "../wificell.h"
#include "../wifilist.h"
#include "../cli.h"
#include "../uiobj.h"
#include "../touchinput.h" // Todo: Update touchInput to byref
#include "scan.h"
#include "screens.h"

using namespace std;

const int btnUpArrowPosY = 5;
const int btnSelectPosY = 13;
const int btnDownArrowPosY = 21;

const int characterPixelSize = 100;
const int wifiRowCount = 2;
const int screenStartingBoundX = 1344;
const int screenEndingBoundX = 3743;
const int screenStartingBoundY = 466;

int ScanScreen::listOffset = 0;
int ScanScreen::selectedListItem = -1;
int ScanScreen::scanState = -1;

WINDOW * ScanScreen::maintty;
vector<UIObject> ScanScreen::uiObjects;
vector<WifiCell> ScanScreen::cellList;

void * ScanScreen::scanArea(void *threadID) {
  if (ScanScreen::scanState == -1 || ScanScreen::scanState == 0) {
    ScanScreen::scanState = 1;
    WifiList * wifiList = new WifiList();
    
    wifiList->wifiScan();
    ScanScreen::cellList = wifiList->getWifiList();

    delete wifiList;
    ScanScreen::scanState = 0;
  }

}

void ScanScreen::updateWindow(vector<int> touchEvents) {
    clear();
    ScanScreen::drawBorder();

    ScanScreen::checkTouchEvents(touchEvents);
    ScanScreen::generateUIObjects();

    if (ScanScreen::scanState == 0) {
      pthread_t scanThread;
      int updateScans = pthread_create(&scanThread, NULL, ScanScreen::scanArea, (void *)0);
    } else if (ScanScreen::scanState <= -1) {
      pthread_t scanThread;
      int updateScans = pthread_create(&scanThread, NULL, ScanScreen::scanArea, (void *)0);
      usleep(3000000);
      return ;
    }

    mvaddstr(1, 2, "Wifi List: (Press to Select).");
    mvaddstr(1, 36, (string("Total in Range: ") + string(CLI::convertInt(ScanScreen::cellList.size()))).c_str());
    int j = 0;
    for(vector<string>::size_type i = ScanScreen::listOffset; i < ScanScreen::cellList.size() && j <= MAX_WIFI_LIST; i++, j++) {
      string tmpWifiMAC = "(" + ScanScreen::cellList[i].getMAC() + ") ";
      int tmpWifiLength = tmpWifiMAC.size();
      string tmpWifiName = " - " + ScanScreen::cellList[i].getESSID();

      if (ScanScreen::selectedListItem == j) {
        attron(COLOR_PAIR(6));
        mvaddstr((3 * (j + 1)), 2, tmpWifiMAC.c_str());
        attroff(COLOR_PAIR(6));
        attron(COLOR_PAIR(7));
        mvaddstr((3 * (j + 1)), tmpWifiLength + 1, tmpWifiName.c_str());
        attroff(COLOR_PAIR(7));
        attron(COLOR_PAIR(6));
        mvaddstr(((3 * (j + 1)) + 1), 2, ScanScreen::cellList[i].getLinkQuality().c_str());
        attroff(COLOR_PAIR(6));
      } else {
        mvaddstr((3 * (j + 1)), 2, tmpWifiMAC.c_str());
        attron(COLOR_PAIR(2));
        mvaddstr((3 * (j + 1)), tmpWifiLength + 1, tmpWifiName.c_str());
        attroff(COLOR_PAIR(2));
        mvaddstr(((3 * (j + 1)) + 1), 2, ScanScreen::cellList[i].getLinkQuality().c_str());
      }
      
      int currentIndex = j;
      int objBottom = (screenStartingBoundY + (j * (characterPixelSize * (wifiRowCount + 1))));

      ScanScreen::uiObjects.push_back(UIObject((string("wifiSignal") + CLI::convertInt(j)), screenStartingBoundX, objBottom, screenEndingBoundX, (objBottom + (characterPixelSize * wifiRowCount)), bind(ScanScreen::btnClickNetwork, currentIndex)));

    }

    drawControls();
    drawExit();

    refresh();
}

void ScanScreen::generateUIObjects() {
  // Possibly needed to avoid memory leak.
  //for(vector<UIObject>::size_type i = 0; i != ScanScreen::uiObjects.size(); i++) {
  //  delete ScanScreen::uiObjects[i];
  //}

  ScanScreen::uiObjects.clear();

  ScanScreen::uiObjects.push_back(UIObject("btnUp", 193, 692, 797, 1056, &ScanScreen::btnUp));
  ScanScreen::uiObjects.push_back(UIObject("btnSelect", 200, 1461, 810, 1773, &ScanScreen::btnSelect));
  ScanScreen::uiObjects.push_back(UIObject("btnDown", 228, 2214, 802, 2549, &ScanScreen::btnDown));

  ScanScreen::uiObjects.push_back(UIObject("btnExit", 221, 3386, 731, 3722, &ScanScreen::btnExit));
}

void ScanScreen::checkTouchEvents(vector<int> touchEvents) {
  if (touchEvents[2] == 0) {
    return ;
  }

  // Todo: Update touchInput to byref
  TouchInput::eventProcessed();
  touchEvents[2] = 0;

  for(vector<UIObject>::size_type i = 0; i != ScanScreen::uiObjects.size(); i++) {
    if (ScanScreen::uiObjects.at(i).collisionDetection(touchEvents[0], touchEvents[1])) {
      ScanScreen::uiObjects.at(i).triggerClickEvent();
    }
  }
}

void ScanScreen::btnClickNetwork(int clickIndex) {
  ScanScreen::selectedListItem = (clickIndex);
}

void ScanScreen::btnUp() {
  if (ScanScreen::selectedListItem <= 0) {
    ScanScreen::selectedListItem = 0;
    if (ScanScreen::listOffset >= 1) {
      ScanScreen::listOffset--;
    } else {
      ScanScreen::listOffset = 0;
    }
  } else {
    ScanScreen::selectedListItem--;
  }
}

void ScanScreen::btnSelect() {
  ScreenHandler::changeScreen(1);
}

void ScanScreen::btnDown() {
  if (ScanScreen::selectedListItem >= MAX_WIFI_LIST) {
    ScanScreen::selectedListItem = MAX_WIFI_LIST;
    if (ScanScreen::listOffset < cellList.size() - MAX_WIFI_LIST) {
      ScanScreen::listOffset++;
    }
  } else {
    ScanScreen::selectedListItem++;
  }
}

void ScanScreen::btnExit() {
  delwin(ScanScreen::maintty);
  endwin();
  exit(0);
}

void ScanScreen::drawExit() {
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

void ScanScreen::drawControls() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    vector<string> btnUpArrow;
    btnUpArrow.push_back(" -------- ");
    btnUpArrow.push_back("|        |");
    btnUpArrow.push_back("| \u25B2\u25B2\u25B2\u25B2\u25B2\u25B2 |");
    btnUpArrow.push_back("|        |");
    btnUpArrow.push_back(" -------- ");

    vector<string> btnSelect;
    btnSelect.push_back(" -------- ");
    btnSelect.push_back("|        |");
    btnSelect.push_back("| SELECT |");
    btnSelect.push_back("|        |");
    btnSelect.push_back(" -------- ");

    vector<string> btnDownArrow;
    btnDownArrow.push_back(" -------- ");
    btnDownArrow.push_back("|        |");
    btnDownArrow.push_back("| \u25BC\u25BC\u25BC\u25BC\u25BC\u25BC |");
    btnDownArrow.push_back("|        |");
    btnDownArrow.push_back(" -------- ");

    for(unsigned int i = 0; i < btnUpArrow.size(); i++ ) {
      attron(COLOR_PAIR(5));
      mvaddstr((btnUpArrowPosY + i), (w.ws_col - btnUpArrow[0].size()) - 1, btnUpArrow[i].c_str());
      attroff(COLOR_PAIR(5));
    }

    for(unsigned int i = 0; i < btnSelect.size(); i++ ) {
      attron(COLOR_PAIR(5));
      mvaddstr((btnSelectPosY + i), (w.ws_col - btnSelect[0].size()) - 1, btnSelect[i].c_str());
      attroff(COLOR_PAIR(5));
    }

    for(unsigned int i = 0; i < btnDownArrow.size(); i++ ) {
      attron(COLOR_PAIR(5));
      mvaddstr((btnDownArrowPosY + i), (w.ws_col - btnDownArrow[0].size()) - 1, btnDownArrow[i].c_str());
      attroff(COLOR_PAIR(5));
    }

}

void ScanScreen::drawBorder() {
  attron(COLOR_PAIR(1));
  box(ScanScreen::maintty, '|', '-');
  attroff(COLOR_PAIR(1));
}
