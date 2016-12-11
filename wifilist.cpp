#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "wifilist.h"
#include "wificell.h"
#include "cli.h"

using namespace std;

const string WifiList::CMD_GETWIFILIST_1 = "iwlist ";
const string WifiList::CMD_GETWIFILIST_2 = " scan | grep 'ESSID\\|Address\\|Quality\\|Frequency\\|Encryption' | tr -s \" \" | awk '{print $0,\",\"}' | sed 's/ Cell /-- \\n Cell /g'";
const string WifiList::DEFAULT_ADAPTER = "wlan0";
const string WifiList::DEFAULT_CELL_DELIMITER = "--";

const string WifiList::PARSE_CELL_1 = " Cell ";
const string WifiList::PARSE_CELL_2 = " - Address";
const string WifiList::PARSE_ADDRESS_1 = " - Address: ";
const string WifiList::PARSE_ADDRESS_2 = " ,";
const string WifiList::PARSE_ESSID_1 = "ESSID:\"";
const string WifiList::PARSE_ESSID_2 = "\" ,";
const string WifiList::PARSE_CHANNEL_1 = "(Channel ";
const string WifiList::PARSE_CHANNEL_2 = ") ,";
const string WifiList::PARSE_FREQUENCY_1 = " Frequency:";
const string WifiList::PARSE_FREQUENCY_2 = " (Channel";
const string WifiList::PARSE_LINK_1 = " Quality=";
const string WifiList::PARSE_LINK_2 = " Signal level";
const string WifiList::PARSE_SIGNAL_1 = " Signal level=";
const string WifiList::PARSE_SIGNAL_2 = " dBm  ,";
const string WifiList::PARSE_ENCRYPTION_1 = " Encryption key:";
const string WifiList::PARSE_ENCRYPTION_2 = " ,";

WifiList::WifiList() {

}

vector<WifiCell> WifiList::getWifiList() {
  return this->wifiList;
}

void WifiList::wifiScan(string adapter) {
  string cmdToRun = WifiList::CMD_GETWIFILIST_1 + adapter + WifiList::CMD_GETWIFILIST_2;

  string cmdResult = CLI::exec(cmdToRun.c_str());

  vector<string> cells = WifiList::parseResultToCells(cmdResult);

  this->wifiList.clear();

  for(vector<string>::size_type i = 0; i != cells.size(); i++) {
    WifiList::parseCommandLineToWifiCell(cells[i]);
  }

}

void WifiList::parseCommandLineToWifiCell(string commandLineCell) {

  if (commandLineCell.size() < 4) {
    return;
  }

  string cellNumber = "Cell ";
  cellNumber += WifiList::parseData(WifiList::PARSE_CELL_1, WifiList::PARSE_CELL_2, commandLineCell);

  string MAC = WifiList::parseData(WifiList::PARSE_ADDRESS_1, WifiList::PARSE_ADDRESS_2, commandLineCell);
  string ESSID = WifiList::parseData(WifiList::PARSE_ESSID_1, WifiList::PARSE_ESSID_2, commandLineCell);
  string channel = WifiList::parseData(WifiList::PARSE_CHANNEL_1, WifiList::PARSE_CHANNEL_2, commandLineCell);
  string frequency = WifiList::parseData(WifiList::PARSE_FREQUENCY_1, WifiList::PARSE_FREQUENCY_2, commandLineCell);
  string linkQuality = WifiList::parseData(WifiList::PARSE_LINK_1, WifiList::PARSE_LINK_2, commandLineCell);
  string signalLevel = WifiList::parseData(WifiList::PARSE_SIGNAL_1, WifiList::PARSE_SIGNAL_2, commandLineCell);
  string encryptionState = WifiList::parseData(WifiList::PARSE_ENCRYPTION_1, WifiList::PARSE_ENCRYPTION_2, commandLineCell);

  this->wifiList.push_back(WifiCell(cellNumber.c_str(), ESSID.c_str(), channel.c_str(), MAC.c_str(), frequency.c_str(), linkQuality.c_str(), signalLevel.c_str(), encryptionState.c_str()));
   
}

vector<string> WifiList::parseResultToCells(string cmdResults) {
  // This function splits the returned results from the command line up into their respective cells.
  vector<string> cells;
  size_t lastPos = 0;
  size_t nextPos = cmdResults.find(WifiList::DEFAULT_CELL_DELIMITER);

  while (nextPos != string::npos) {
    cells.push_back(cmdResults.substr(lastPos, nextPos - lastPos));
    lastPos = nextPos + WifiList::DEFAULT_CELL_DELIMITER.length();
    nextPos = cmdResults.find(WifiList::DEFAULT_CELL_DELIMITER, lastPos);
  }
  
  cells.push_back(cmdResults.substr(lastPos, nextPos));

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

void WifiList::wifiScan() {
  WifiList::wifiScan(WifiList::DEFAULT_ADAPTER);
}




