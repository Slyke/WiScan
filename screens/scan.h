#ifndef __SCAN_SCREEN_H_INCLUDED__
#define __SCAN_SCREEN_H_INCLUDED__ 

using namespace std;

#define MAX_WIFI_LIST    12

class ScanScreen
{
  public:
    static void updateWindow(vector<int> touchEvents);
    static void drawExit();
    static void drawControls();
    static WINDOW * maintty;

  private:
    static void drawBorder();
    static void generateUIObjects();
    static vector<UIObject> uiObjects;

};


#endif

