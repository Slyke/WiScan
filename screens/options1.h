#ifndef __OPTIONS1_SCREEN_H_INCLUDED__
#define __OPTIONS1_SCREEN_H_INCLUDED__ 

#include <vector>
#include <string>
#include <curses.h>

#include "../uiobj.h"

using namespace std;

class Options1
{
  public:
    static void updateWindow(vector<int> touchEvents);
    static WINDOW * maintty;

  private:
    static const string DEFAULT_ADAPTER;
    static const string CMD_GETNETWORKINFO_1;
    static const string CMD_GETNETWORKINFO_IP;
    static const string CMD_GETNETWORKINFO_BCAST;
    static const string CMD_GETNETWORKINFO_MASK;
    static const string CMD_GETNETWORKINFO_MAC;
    static vector<UIObject> uiObjects;
    static void generateUIObjects();
    static void setupKeyboardEvents();
    static void drawBorder();
    static void checkTouchEvents(vector<int> touchEvents);
    static void btnExit();
    static void btnBack();
    static void drawExit();
    static void drawBack();
    static void drawControls();
    static string networkDetailsIP(string adapter);
    static string networkDetailsIP();
    static string networkDetailsMAC(string adapter);
    static string networkDetailsMAC();
    static string networkDetailsBCAST(string adapter);
    static string networkDetailsBCAST();
    static string networkDetailsMASK(string adapter);
    static string networkDetailsMASK();
    
    

};
#endif

