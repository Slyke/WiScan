#include <stdio.h>
#include <string>

#include "uiobj.h"

using namespace std;

UIObject::UIObject(string uiName, int x1, int y1, int x2, int y2) {

  this->uiName = uiName;
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;

}

bool UIObject::collisionDetection(int x, int y) {
  if (x >= this->x1 && x <= x2 && this->y1 >= y && this->y2 <= y) {
    return true;
  }
  return false;
}




