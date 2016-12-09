#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <cstdio>
#include <string>

#include "cli.h"

using namespace std;

WINDOW * maintty;

int main (void)
{
  if ((maintty = initscr()) == NULL) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(1);
  }
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  //iwlist wlan0 scan | grep "ESSID:" | cut -d':' -f2- // WIFI LIST
  mvaddstr(1, 1, "123456789-123456789-123456789-123456789-123456789-123456789");
  mvaddstr(5, 25, "Testing");
  string thecmd = "iwlist wlan0 scan | grep \"ESSID:\" | cut -d':' -f2-";
  string result = CLI::exec(thecmd.c_str());
  mvaddstr(6, 2, result.c_str());
  refresh();
  sleep(3);

  delwin(maintty);
  endwin();
  return 0;
}




