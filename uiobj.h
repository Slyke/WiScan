#ifndef __UIOBJ_H_INCLUDED__
#define __UIOBJ_H_INCLUDED__ 

#include <stdio.h>
#include <string>
#include <functional>

using namespace std;

class UIObject
{
  public:
    UIObject(string uiName, int x1, int y1, int x2, int y2);
    UIObject(string uiName, int x1, int y1, int x2, int y2, function<void()> clickEvent);
    // UIObject(string uiName, int x1, int y1, int x2, int y2, function<void()> clickEvent, int clickParam);
    bool collisionDetection(int x, int y);
    int x1;
    int y1;
    int x2;
    int y2;
    string uiName;
    function<void()> clickEvent;
    void triggerClickEvent();
    // int clickParam;
};

#endif
