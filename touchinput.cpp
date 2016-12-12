#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <array>

#include "touchinput.h"

#define EVENT_DEVICE    "/dev/input/event2"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y

// int TouchInput::touchEvent = new int[3];

void TouchInput::updateTouchInputs() {
  struct input_event ev;
  int fd;
  char name[256] = "Unknown";

  if ((getuid ()) != 0) {
    fprintf(stderr, "You are not root! This may not work...\n");
    return EXIT_SUCCESS;
  }

  fd = open(EVENT_DEVICE, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
    return EXIT_FAILURE;
  }

  ioctl(fd, EVIOCGNAME(sizeof(name)), name);
  // printf("Reading from:\n");
  // printf("device file = %s\n", EVENT_DEVICE);
  // printf("device name = %s\n", name);

  for (;;) {
    const size_t ev_size = sizeof(struct input_event);
    ssize_t size;

    size = read(fd, &ev, ev_size);
    if (size < ev_size) {
      fprintf(stderr, "Error size when reading\n");
      goto err;
    }

    if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X || ev.code == EVENT_CODE_Y)) {
      if (ev.code == EVENT_CODE_X) {
        TouchInput::touchEvent = {ev.value, TouchInput::touchEvent[1], TouchInput::touchEvent[2]};
      } else if (ev.code == EVENT_CODE_Y) {
        TouchInput::touchEvent = {TouchInput::touchEvent[0], ev.value, TouchInput::touchEvent[2]};
      }
      
    //   printf("%s = %d\n", ev.code == EVENT_CODE_X ? "X" : "Y", ev.value);
    }
  }

//   close(fd);
  return ;

  err:
   close(fd);
}

array<int> TouchInput::getTouchEvent() {
    return TouchInput::touchEvent;
}
