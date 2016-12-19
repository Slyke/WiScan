#ifndef __TOUCH_INPUT_H_INCLUDED__
#define __TOUCH_INPUT_H_INCLUDED__ 

#include <vector>

using namespace std;

class TouchInput
{
  public:
    static void updateTouchInputs();
    static vector<int> getTouchInput();
    static void eventProcessed();
    static void findTouchDevice();
    static string touchDevice;

  private:
    static void drawBorder();
    static vector<int> touchEvent;
    

};


#endif

