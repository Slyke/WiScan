#include <stdio.h>
#include <iostream>
#include <string>

#include "wificell.h"

using namespace std;

WifiCell::WifiCell(const char* cellID, const char* ESSID, const char* channel, const char* MAC, const char* frequency, const char* linkQuality, const char* signalLevel, const char* encryption) {

  this->cellID = cellID;
  this->ESSID = ESSID;
  this->channel = channel;
  this->MAC = MAC;
  this->frequency = frequency;
  this->linkQuality = linkQuality;
  this->signalLevel = signalLevel;
  this->encryption = encryption;

}

string WifiCell::getCellID() {
  return cellID;
}

string WifiCell::getESSID() {
  return ESSID;
}

string WifiCell::getChannel() {
  return channel;
}

string WifiCell::getMAC() {
  return MAC;
}

string WifiCell::getFrequency() {
  return frequency;
}

string WifiCell::getLinkQuality() {
  return linkQuality;
}

string WifiCell::getSignalLevel() {
  return signalLevel;
}

string WifiCell::getEncryption() {
  return encryption;
}


