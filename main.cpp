#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <cstdio>
#include <string>
#include <sys/ioctl.h>

#include <pthread.h>
#include <cstdlib>

#include "screens/screens.h"
#include "touchinput.h"

using namespace std;

WINDOW * maintty;
bool appRunning = true;

void * checkTouchEvents(void *threadID) {
  while (appRunning) {
    TouchInput::updateTouchInputs();
    usleep(20000);
  }
}

void *UpdateWindow(void *threadID) {

  while (appRunning) {
    ScreenHandler::updateScreen(maintty);
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    mvaddstr(w.ws_row - 1, w.ws_col - 1, "");
    refresh();
    usleep(2000);
  }

  pthread_exit(NULL);

}

void setupWindow() {

  start_color();

  init_color(COLOR_BLACK, 0, 0, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK); // Border color
  init_pair(2, COLOR_GREEN, COLOR_BLACK); // Wifi Name
  init_pair(3, COLOR_CYAN, COLOR_RED); // Exit Background
  init_pair(4, COLOR_GREEN, COLOR_RED); // Exit text
  init_pair(5, COLOR_WHITE, COLOR_BLUE); // Buttons

  init_pair(6, COLOR_GREEN, COLOR_BLUE); // Selected Item
  init_pair(7, COLOR_WHITE, COLOR_BLUE); // Selected Wifi Name
  init_pair(8, COLOR_CYAN, COLOR_BLACK); // Exit Background
  
  attron(COLOR_PAIR(1));
  box(maintty, '|', '-');
  attroff(COLOR_PAIR(1));
  touchwin(maintty);
  wrefresh(maintty);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

int main (void)
{
  setlocale(LC_ALL, "");
  if ((maintty = initscr()) == NULL) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(1);
  }
  if (has_colors() == FALSE)	{
    endwin();
    fprintf(stderr, "Your terminal does not support color\n");
    exit(1);
  }
  TouchInput::findTouchDevice();
  setupWindow();

  mvaddstr(8, 8, "Setting up and getting the list...");
  refresh();

  pthread_t uiThread;
  pthread_t touchThread;

  int uiUpdate = pthread_create(&uiThread, NULL, UpdateWindow, (void *)0);
  int uiTouchEvent = pthread_create(&touchThread, NULL, checkTouchEvents, (void *)0);

  while (appRunning) {
    sleep(20); // Let the other threads run the execution now.
  }

  delwin(maintty);
  endwin();
  return 0;
}




