#ifndef __CELL_SCREEN_H_INCLUDED__
#define __CELL_SCREEN_H_INCLUDED__ 

#include <vector>
#include <string>
#include <curses.h>

#include "../uiobj.h"
#include "../wificell.h"

using namespace std;

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
    static void btnBack();
    static void drawExit();
    static void drawBack();
    static void drawControls();
    

};
#endif

