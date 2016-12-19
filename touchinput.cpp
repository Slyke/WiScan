#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <vector>
#include <string>
#include <string.h>

#include "touchinput.h"
#include "cli.h"

using namespace std;

#define EVENT_DEVICE    "/dev/input/event"
#define TOUCH_DEVICE    "stmpe-ts"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y
#define EVENT_DEVICE_LOW  1
#define EVENT_DEVICE_HIGH 10

vector<int> TouchInput::touchEvent;
string TouchInput::touchDevice;

void TouchInput::findTouchDevice() {

  int fd;
  char name[256] = "Unknown";

  if ((getuid ()) != 0) {
    fprintf(stderr, "You are not root! This may not work...\n");
    return ;
  }

  string currentDevice;
  for (int i = EVENT_DEVICE_LOW; i < EVENT_DEVICE_HIGH; i++) {
    currentDevice = string(EVENT_DEVICE + CLI::convertInt(i));

    fd = open(currentDevice.c_str(), O_RDONLY);
    if (fd == -1) {
      close(fd);
      continue;
    }

    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    close(fd);

    if (strcmp(name, TOUCH_DEVICE) == 0) {
      TouchInput::touchDevice = currentDevice;
      break;
    }
  }

  if (TouchInput::touchDevice == "") {
    fprintf(stderr, "No touch devices found. Touch will not work.\n");
  }

}

void TouchInput::updateTouchInputs() {
  struct input_event ev;
  int fd;

  if (TouchInput::touchDevice == "") {
    return ;
  }

  fd = open(TouchInput::touchDevice.c_str(), O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "%s is not a vaild device. Touch will not work.\n", TouchInput::touchDevice.c_str());
    return ;
  }

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
