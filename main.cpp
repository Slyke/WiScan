#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <cstdio>
#include <string>

#include "cli.h"
#include "wificell.h"
#include "wifilist.h"

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
  mvaddstr(1, 1, "123456789-123456789-123456789-123456789-123456789-123456789");
  mvaddstr(5, 25, "Testing");

  WifiList * wifiList = new WifiList();
  wifiList->wifiScan();


  mvaddstr(8, 16, wifiList->getWifiList().at(0).getMAC().c_str());

  refresh();
  sleep(3);

  delwin(maintty);
  endwin();
  return 0;
}




