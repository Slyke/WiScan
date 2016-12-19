#include <stdio.h>
#include <curses.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>


#include "../cli.h"
#include "../uiobj.h"
#include "../touchinput.h" // Todo: Update touchInput to byref
#include "options1.h"
#include "screens.h"

const string Options1::DEFAULT_ADAPTER = "wlan0";
const string Options1::CMD_GETNETWORKINFO_1 = "ifconfig ";
const string Options1::CMD_GETNETWORKINFO_IP = " | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'";
const string Options1::CMD_GETNETWORKINFO_BCAST = " | grep 'inet addr:' | cut -d: -f3 | awk '{ print $1}'";
const string Options1::CMD_GETNETWORKINFO_MASK = " | grep 'inet addr:' | cut -d: -f4 | awk '{ print $1}' | tr '\n' ' '";
const string Options1::CMD_GETNETWORKINFO_MAC = " | grep 'HWaddr' | awk '{print $5}' | tr '\n' ' '";

vector<UIObject> Options1::uiObjects;
WINDOW * Options1::maintty;

void Options1::updateWindow(vector<int> touchEvents) {
    clear();
    Options1::drawBorder();

    Options1::checkTouchEvents(touchEvents);
    Options1::generateUIObjects();
        
    string adapter = Options1::DEFAULT_ADAPTER;
    attron(COLOR_PAIR(8));
    mvaddstr(1, 1, string("Currently Connected With:  ").c_str());
    mvaddstr(2, 3, string("IP    : ").c_str());
    mvaddstr(3, 3, string("MASK  : 000.              BCAST: ").c_str());
    mvaddstr(4, 3, string("MAC   : ").c_str());
    mvaddstr(6, 1, string("TouchScreen Device   : ").c_str());
    attroff(COLOR_PAIR(8));

    attron(COLOR_PAIR(2));
    mvaddstr(1, 28, adapter.c_str());
    mvaddstr(2, 11, Options1::networkDetailsIP(adapter).c_str());
    mvaddstr(3, 11, string(Options1::networkDetailsMASK(adapter)).c_str());
    mvaddstr(3, 36, string(Options1::networkDetailsBCAST(adapter)).c_str());
    mvaddstr(4, 11, string(Options1::networkDetailsMAC(adapter)).c_str());
    mvaddstr(6, 24, string(TouchInput::touchDevice).c_str());
    attroff(COLOR_PAIR(2));

    Options1::drawExit();
    Options1::drawBack();
    refresh();
}

void Options1::generateUIObjects() {
  // Possibly needed to avoid memory leak.
  //for(vector<UIObject>::size_type i = 0; i != ScanScreen::uiObjects.size(); i++) {
  //  delete ScanScreen::uiObjects[i];
  //}

  Options1::uiObjects.clear();

  Options1::uiObjects.push_back(UIObject("btnExit", 221, 3386, 731, 3722, &Options1::btnExit));
  Options1::uiObjects.push_back(UIObject("btnBack", 3012, 3386, 3799, 3722, &Options1::btnBack));
}

void Options1::checkTouchEvents(vector<int> touchEvents) {
  if (touchEvents[2] == 0) {
    return ;
  }

  // Todo: Update touchInput to byref
  TouchInput::eventProcessed();
  touchEvents[2] = 0;

  for(vector<UIObject>::size_type i = 0; i != Options1::uiObjects.size(); i++) {
    if (Options1::uiObjects.at(i).collisionDetection(touchEvents[0], touchEvents[1])) {
      Options1::uiObjects.at(i).triggerClickEvent();
    }
  }
}

void Options1::drawExit() {
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

void Options1::btnExit() {
  delwin(Options1::maintty);
  endwin();
  exit(0);
}

void Options1::drawBorder() {
  attron(COLOR_PAIR(1));
  box(Options1::maintty, '|', '-');
  attroff(COLOR_PAIR(1));
}

void Options1::drawBack() {
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

void Options1::btnBack() {
  ScreenHandler::changeScreen(0);
}

string  Options1::networkDetailsIP() {
  return Options1::networkDetailsIP(Options1::DEFAULT_ADAPTER);
}

string Options1::networkDetailsIP(string adapter) {
  string cmdToRun = Options1::CMD_GETNETWORKINFO_1 + adapter + Options1::CMD_GETNETWORKINFO_IP;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  return cmdResult;
}

string  Options1::networkDetailsMASK() {
  return Options1::networkDetailsMASK(Options1::DEFAULT_ADAPTER);
}

string Options1::networkDetailsMASK(string adapter) {
  string cmdToRun = Options1::CMD_GETNETWORKINFO_1 + adapter + Options1::CMD_GETNETWORKINFO_MASK;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  return cmdResult;
}

string  Options1::networkDetailsBCAST() {
  return Options1::networkDetailsBCAST(Options1::DEFAULT_ADAPTER);
}

string Options1::networkDetailsBCAST(string adapter) {
  string cmdToRun = Options1::CMD_GETNETWORKINFO_1 + adapter + Options1::CMD_GETNETWORKINFO_BCAST;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  return cmdResult;
}

string  Options1::networkDetailsMAC() {
  return Options1::networkDetailsMAC(Options1::DEFAULT_ADAPTER);
}

string Options1::networkDetailsMAC(string adapter) {
  string cmdToRun = Options1::CMD_GETNETWORKINFO_1 + adapter + Options1::CMD_GETNETWORKINFO_MAC;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  return cmdResult;
}
