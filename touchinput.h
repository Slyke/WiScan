#ifndef __TOUCH_INPUT_H_INCLUDED__
#define __TOUCH_INPUT_H_INCLUDED__ 

#include <array>

class TouchInput
{
  public:
    static void updateTouchInputs();
    static array<int> getTouchEvent();

  private:
    static void drawBorder();
    static array<int> touchEvent;

};


#endif

