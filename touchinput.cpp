#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <vector>

#include "touchinput.h"

using namespace std;


#define EVENT_DEVICE    "/dev/input/event2"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y

vector<int> TouchInput::touchEvent;

void TouchInput::updateTouchInputs() {
  struct input_event ev;
  int fd;
  char name[256] = "Unknown";

  if ((getuid ()) != 0) {
    fprintf(stderr, "You are not root! This may not work...\n");
    return ;
  }

  fd = open(EVENT_DEVICE, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
    return ;
  }

  ioctl(fd, EVIOCGNAME(sizeof(name)), name);
   for (int i = 0; i < 5; i++) {
     const size_t ev_size = sizeof(struct input_event);
     ssize_t size;

     size = read(fd, &ev, ev_size);
     if (size < ev_size) {
       fprintf(stderr, "Error size when reading\n");
       goto err;
     }

     if (TouchInput::touchEvent.size() != 3) {
       TouchInput::touchEvent.clear();
       TouchInput::touchEvent.push_back(-1);
       TouchInput::touchEvent.push_back(-1);
       TouchInput::touchEvent.push_back(-1);
     }

     if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X || ev.code == EVENT_CODE_Y)) {
       if (ev.code == EVENT_CODE_X) {
         TouchInput::touchEvent[1] = ev.value;
       } else if (ev.code == EVENT_CODE_Y) {
         TouchInput::touchEvent[0] = ev.value;
       }
       TouchInput::touchEvent[2] =  1; // Let the UI know there's been an update.
     }
   }

  close(fd);
  return ;

  err:
   close(fd);
}

void TouchInput::eventProcessed() {
  TouchInput::touchEvent[2] =  0;
}

vector<int> TouchInput::getTouchInput() {

    if (TouchInput::touchEvent.size() != 3) {
      TouchInput::touchEvent.clear();
      TouchInput::touchEvent.push_back(-1);
      TouchInput::touchEvent.push_back(-1);
      TouchInput::touchEvent.push_back(-1);
    }

    return TouchInput::touchEvent;
}
