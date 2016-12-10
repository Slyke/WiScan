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
    void wifiScan(string adapter);
    vector<WifiCell> getWifiList();

  private:
    static const string CMD_GETWIFILIST_1;
    static const string CMD_GETWIFILIST_2;
    static const string DEFAULT_ADAPTER;
    static const string DEFAULT_CELL_DELIMITER;
    static const string PARSE_CELL_1;
    static const string PARSE_CELL_2;
    static const string PARSE_ADDRESS_1;
    static const string PARSE_ADDRESS_2;
    static vector<string> parseResultToCells(string cmdResults);
    void parseCommandLineToWifiCell(string commandLineCell);
    static string convertInt(int number);
    static string parseData(string startDelimiter, string endDelimiter, string fromString);
    vector<WifiCell> wifiList;

};

#endif
