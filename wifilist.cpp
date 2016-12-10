#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

#include "wifilist.h"
#include "wificell.h"
#include "cli.h"

using namespace std;

const string WifiList::CMD_GETWIFILIST_1 = "iwlist ";
const string WifiList::CMD_GETWIFILIST_2 = " scan | grep 'ESSID\\|Address\\|Quality\\|Frequency\\|Encryption' | tr -s \" \" | awk '{print $0,\",\"}' | sed 's/ Cell /-- \\n Cell /g'";
const string WifiList::DEFAULT_ADAPTER = "wlan0";
const string WifiList::DEFAULT_CELL_DELIMITER = "--\n";

const string WifiList::PARSE_CELL_1 = " Cell ";
const string WifiList::PARSE_CELL_2 = " - Address";
const string WifiList::PARSE_ADDRESS_1 = " - Address: ";
const string WifiList::PARSE_ADDRESS_2 = " ,";

WifiList::WifiList() {

}

vector<WifiCell> WifiList::getWifiList() {
  return this->wifiList;
}

void WifiList::wifiScan(string adapter) {
  string cmdToRun = WifiList::CMD_GETWIFILIST_1 + adapter + WifiList::CMD_GETWIFILIST_2;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  vector<string> cells = WifiList::parseResultToCells(cmdResult);

  for(vector<string>::size_type i = 0; i != cells.size(); i++) {
    WifiList::parseCommandLineToWifiCell(cells[i]);
  }

}

void WifiList::parseCommandLineToWifiCell(string commandLineCell) {
  string cellNumber = "Cell ";
  cellNumber += WifiList::parseData(WifiList::PARSE_CELL_1, WifiList::PARSE_CELL_2, commandLineCell);

  string MAC = WifiList::parseData(WifiList::PARSE_ADDRESS_1, WifiList::PARSE_ADDRESS_2, commandLineCell);

  this->wifiList.push_back(WifiCell(cellNumber.c_str(), "essid1", "channel", MAC.c_str(), "frequency", "link", "signal", "encryption"));

    
}

vector<string> WifiList::parseResultToCells(string cmdResults) {
  // This function splits the returned results from the command line up into their respective cells.
  vector<string> cells;
  size_t lastPos = 0;
  size_t nextPos = 0;
  string token;
  while ((nextPos = cmdResults.find(WifiList::DEFAULT_CELL_DELIMITER)) != string::npos) {
    cells.push_back(cmdResults.substr(lastPos, nextPos - lastPos));
    lastPos = nextPos + 1;
  }
  
  cells.push_back(cmdResults.substr(lastPos));

  return cells;

}

string WifiList::parseData(string startDelimiter, string endDelimiter, string fromString) {

  unsigned firstOccurance = fromString.find(startDelimiter);
  firstOccurance += startDelimiter.size();
  unsigned secondOccurance = fromString.find(endDelimiter);
  //unsigned lastOccurance = fromString.find_last_of(endDelimiter);
  string theReturn = fromString.substr (firstOccurance, secondOccurance - firstOccurance);

  return theReturn;

}

string WifiList::convertInt(int number) {
   stringstream ss;
   ss << number;
   return ss.str();
}

void WifiList::wifiScan() {
  WifiList::wifiScan(WifiList::DEFAULT_ADAPTER);
}




