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
#include "scan.h"

using namespace std;

const int btnUpArrowPosY = 5;
const int btnSelectPosY = 13;
const int btnDownArrowPosY = 21;

WINDOW * ScanScreen::maintty;
vector<UIObject> ScanScreen::uiObjects;

void ScanScreen::updateWindow(vector<int> touchEvents) {
    clear();
    ScanScreen::drawBorder();

    ScanScreen::generateUIObjects();

    WifiList * wifiList = new WifiList();
    vector<WifiCell> cellList;
    
    wifiList->wifiScan();
    cellList = wifiList->getWifiList();

    mvaddstr(1, 2, "Wifi List: (Press to Select).");
    mvaddstr(1, 36, (string("Total in Range: ") + string(CLI::convertInt(cellList.size()))).c_str());

    for(vector<string>::size_type i = 0; i != cellList.size() && i != MAX_WIFI_LIST; i++) {
      string tmpWifiMAC = "(" + cellList[i].getMAC() + ") ";
      int tmpWifiLength = tmpWifiMAC.size();
      string tmpWifiName = " - " + cellList[i].getESSID();
      mvaddstr((3 * (i + 1)), 2, tmpWifiMAC.c_str());
      attron(COLOR_PAIR(2));
      mvaddstr((3 * (i + 1)), tmpWifiLength, tmpWifiName.c_str());
      attroff(COLOR_PAIR(2));
      mvaddstr(((3 * (i + 1)) + 1), 2, cellList[i].getLinkQuality().c_str());
    }

    drawControls();
    drawExit();

    refresh();
    delete wifiList;
}

void ScanScreen::generateUIObjects() {
  for(vector<string>::size_type i = 0; i != ScanScreen::uiObjects.size(); i++) {
    delete ScanScreen::uiObjects[i];
  }

  ScanScreen::uiObjects.push_back(UIObject("exit", 221, 3386, 731, 3722));
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