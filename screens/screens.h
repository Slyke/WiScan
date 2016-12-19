#ifndef __SCREEN_H_INCLUDED__
#define __SCREEN_H_INCLUDED__ 

#include "scan.h"
#include "cell.h"

using namespace std;

class ScreenHandler
{
  public:
    static void updateScreen(WINDOW * maintty);
    static void changeScreen(int newScreen);
    static int currentScreen;

};

#endif

