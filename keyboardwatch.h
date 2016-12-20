#ifndef __KEYBOARD_WATCH_H_INCLUDED__
#define __KEYBOARD_WATCH_H_INCLUDED__ 

#include <curses.h>
#include <functional>
#include <iostream>
#include <map>

using namespace std;

class KeyboardWatch
{
  public:
    static void addKey(int keyCode, function<void()> triggerEvent);
    static void removeKey(int keyCode);
    static void checkPresses();
    static void clearKeys();
    static bool continueWatch;

    private:
      static map<int, function<void()>> watchKeys;

};

#endif

