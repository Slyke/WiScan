#include <stdio.h>
#include <iostream>
#include <string>

#include "wificell.h"
#include "cli.h"

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

WifiCell::WifiCell() {

}

string WifiCell::getCellID() {
  return cellID;
}

string WifiCell::getESSID() {
  if (ESSID.size() > 20) {
    return ESSID.substr(0, 20);
  }
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

int WifiCell::getLinkQualityLower() {
  string lower = WifiCell::getLinkQuality();
  lower = lower.substr(0, lower.find("/"));
  return atoi(lower.c_str());
}

int WifiCell::getLinkQualityMax() {
  string upper = WifiCell::getLinkQuality();
  upper = upper.substr(upper.find("/") + 1);
  return atoi(upper.c_str());
}

void WifiCell::setLinkQuality(string newLinkQuality) {
  linkQuality = newLinkQuality;
}

void WifiCell::setSignalLevel(string newSignalStrength) {
  signalLevel = newSignalStrength;
}



