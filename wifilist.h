#ifndef __WIFILIST_H_INCLUDED__
#define __WIFILIST_H_INCLUDED__ 

#include<vector>

#include "wificell.h"
// #include "wifilist.h"

using namespace std;

class WifiList
{
  public:
    WifiList();
    void wifiScan();
    vector<WifiCell> getWifiList();

  private:
    struct CommandLineCommands;
    vector<WifiCell> wifiList;

};

#endif
