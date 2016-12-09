#ifndef __WIFICELL_H_INCLUDED__
#define __WIFICELL_H_INCLUDED__ 

using namespace std;

class WifiCell
{
  public:
    WifiCell(const char* cellID, const char* ESSID, const char* channel, const char* MAC, const char* frequency, const char* linkQuality, const char* signalLevel, const char* encryption);
    string getCellID();
    string getESSID();
    string getChannel();
    string getMAC();
    string getFrequency();
    string getLinkQuality();
    string getSignalLevel();
    string getEncryption();

  private:
    string cellID;
    string ESSID;
    string channel;
    string MAC;
    string frequency;
    string linkQuality;
    string signalLevel;
    string encryption;
};

#endif
