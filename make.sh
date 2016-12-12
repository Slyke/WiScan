#!/bin/bash
 
# Replace with makefile
 
# rm wiscan
 
g++ -lncursesw -lpthread cli.cpp wifilist.cpp wificell.cpp screens/scan.cpp main.cpp -o wiscan
