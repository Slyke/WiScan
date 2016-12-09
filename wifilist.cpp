#include <stdio.h>
#include <iostream>
#include <string>
#include<vector>

#include "wifilist.h"
#include "wificell.h"

using namespace std;

WifiList::WifiList() {

}

vector<WifiCell> WifiList::getWifiList() {
  return this->wifiList;
}

void WifiList::wifiScan() {
  this->wifiList.push_back(WifiCell("Cell1", "essid1", "channel", "mac", "frequency", "link", "signal", "encryption"));
  this->wifiList.push_back(WifiCell("Cell2", "essid2", "channel", "mac", "frequency", "link", "signal", "encryption"));
}


