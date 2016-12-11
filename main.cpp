#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <cstdio>
#include <string>

#include <pthread.h>
#include <cstdlib>

#include "cli.h"
#include "wificell.h"
#include "wifilist.h"

#define MAX_WIFI_LIST    12

using namespace std;

WINDOW * maintty;
WifiList * wifiList;

void *UpdateWindow(void *threadID) {

  wifiList = new WifiList();
  vector<WifiCell> cellList;
  string tmpWifiName;

  mvaddstr(1, 2, "Wifi List: (Press to Select).");

  for (;;) {
    wifiList->wifiScan();
    cellList = wifiList->getWifiList();

    mvaddstr(1, 35, (string("Total in Range: ") + string(CLI::convertInt(cellList.size()))).c_str());

    for(vector<string>::size_type i = 0; i != cellList.size() && i != MAX_WIFI_LIST; i++) {
      tmpWifiName = cellList[i].getESSID() + " - (" + cellList[i].getMAC() + ")";
      mvaddstr((3 * (i + 1)), 2, tmpWifiName.c_str());
      mvaddstr(((3 * (i + 1)) + 1), 2, cellList[i].getLinkQuality().c_str());
      // mvaddstr((4 * (i + 1)), 2,  CLI::convertInt(i).c_str());
    }

    refresh();
    // sleep(1);
  }

  pthread_exit(NULL);

}

int main (void)
{
  if ((maintty = initscr()) == NULL) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(1);
  }
  box(maintty, '|', '-');
  touchwin(maintty);
  wrefresh(maintty);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  // mvaddstr(1, 1, "123456789-123456789-123456789-123456789-123456789-123456789");
  // mvaddstr(5, 25, "Testing");

  // mvaddstr(8, 16, "Getting list...");

  pthread_t uiThread;
  pthread_t touchThread;

  int uiUpdate = pthread_create(&uiThread, NULL, UpdateWindow, (void *)0);
  //int uiTouchEvent = pthread_create(&touchThread, NULL, UpdateWindow, (void *)0);

  refresh();
  sleep(10);

  delwin(maintty);
  endwin();
  return 0;
}




