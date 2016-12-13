#ifndef __UIOBJ_H_INCLUDED__
#define __UIOBJ_H_INCLUDED__ 

#include <stdio.h>
#include <string>

using namespace std;

class UIObject
{
  public:
    UIObject(string uiName, int x1, int y1, int x2, int y2);
    bool collisionDetection(int x, int y);
    int x1;
    int y1;
    int x2;
    int y2;
    string uiName;
};

#endif
