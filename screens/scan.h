#ifndef __SCAN_SCREEN_H_INCLUDED__
#define __SCAN_SCREEN_H_INCLUDED__ 

#include <vector>
#include <curses.h>

#include "../uiobj.h"
#include "../wificell.h"

using namespace std;

#define MAX_WIFI_LIST    9

class ScanScreen
{
  public:
    static void updateWindow(vector<int> touchEvents);
    static void drawExit();
    static void drawControls();
    static void drawOptions();
    static WINDOW * maintty;

  private:
    static void * scanArea(void *threadID);
    static void drawBorder();
    static void generateUIObjects();
    static vector<UIObject> uiObjects;
    static void checkTouchEvents(vector<int> touchEvents);
    static int listOffset;
    static int selectedListItem;
    static vector<WifiCell> cellList;
    static void btnExit();
    static void btnUp();
    static void btnSelect();
    static void btnOptions();
    static void btnDown();
    static void btnClickNetwork(int clickIndex);
    static int scanState;

};
#endif

