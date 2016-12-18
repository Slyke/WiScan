#include <stdio.h>
#include <string>
#include <functional>

#include "uiobj.h"

using namespace std;

UIObject::UIObject(string uiName, int x1, int y1, int x2, int y2) {

  this->uiName = uiName;
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;

}

UIObject::UIObject(string uiName, int x1, int y1, int x2, int y2, function<void()> clickEvent) {

  this->uiName = uiName;
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->clickEvent = clickEvent;

}

/* UIObject::UIObject(string uiName, int x1, int y1, int x2, int y2, function<void()> clickEvent, int clickParam) {

  this->uiName = uiName;
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->clickEvent = clickEvent;
  this->clickParam = clickParam;

} */

bool UIObject::collisionDetection(int x, int y) {
  if (x >= this->x1 && x <= this->x2 &&  y >= this->y1 && y <= this->y2) {
    return true;
  }
  return false;
}

void UIObject::triggerClickEvent() {
  if (this->clickEvent) {
    /* if (this->clickParam) {
      this->clickEvent(this->clickParam);
    } else { */
      this->clickEvent();
    // }

  }
}




