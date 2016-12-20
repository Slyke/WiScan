
#include <curses.h>
#include <stdio.h>
#include <functional>
#include <iostream>
#include <map>

#include "keyboardwatch.h"

using namespace std;

bool KeyboardWatch::continueWatch = true;

map<int, function<void()>> KeyboardWatch::watchKeys;

void KeyboardWatch::addKey(int keyCode, function<void()> triggerEvent) {
  KeyboardWatch::watchKeys[keyCode] = triggerEvent;
}

void KeyboardWatch::removeKey(int keyCode) {
  KeyboardWatch::watchKeys.erase(keyCode);
}

void KeyboardWatch::clearKeys() {
  KeyboardWatch::watchKeys.clear();
}

void KeyboardWatch::checkPresses() {

  int keyPress;

  while (KeyboardWatch::continueWatch) {
    keyPress = getch();

    if (KeyboardWatch::watchKeys.find(keyPress) == KeyboardWatch::watchKeys.end()) {
      continue;
    } else {
      KeyboardWatch::watchKeys[keyPress]();
    }

  }

}
