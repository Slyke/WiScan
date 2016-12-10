#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "wifilist.h"
#include "wificell.h"
#include "cli.h"

using namespace std;

struct CommandLineCommands {
    static map<string, string> create_map()
        {
          map<string, string> m;
          m["WifiList"] = "iwlist wlan0 scan | grep 'ESSID\\|Address\\|Quality\\|Frequency\\|Encryption' | tr -s \" \" | awk '{print $0,\",\"}' | sed 's/ Cell /-- \\n Cell /g'";
          m["c"] = "d";
          return m;
        }
    static map<string, string> commandMap;

};

map<string, string> CommandLineCommands::commandMap = CommandLineCommands::create_map();

WifiList::WifiList() {

}

vector<WifiCell> WifiList::getWifiList() {
  return this->wifiList;
}

void WifiList::wifiScan() {
  string thecmd = "s";
  string cmdResult = CLI::exec(WifiList::CommandLineCommands::commandMap["WifiList"]);
  this->wifiList.push_back(WifiCell("Cell1", "essid1", "channel", "mac", "frequency", "link", "signal", "encryption"));
  this->wifiList.push_back(WifiCell("Cell2", "essid2", "channel", "mac", "frequency", "link", "signal", "encryption"));
}


