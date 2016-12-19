#ifndef __CELL_SCREEN_H_INCLUDED__
#define __CELL_SCREEN_H_INCLUDED__ 

#include <vector>
#include <string>
#include <curses.h>

#include "../uiobj.h"
#include "../wificell.h"

using namespace std;

#define MAX_WIFI_LIST    11

class CellScreen
{
  public:
    static string cellMAC;
    static void updateWindow(vector<int> touchEvents);
    static WINDOW * maintty;

  private:
    static vector<UIObject> uiObjects;
    static void generateUIObjects();
    static void drawBorder();
    static void checkTouchEvents(vector<int> touchEvents);
    static void btnExit();
    static void drawExit();
    static void drawControls();
    

};
#endif

