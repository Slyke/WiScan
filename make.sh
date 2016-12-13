#!/bin/bash
 
# Replace with makefile
 
# rm wiscan
 
g++ cli.cpp wifilist.cpp wificell.cpp screens/scan.cpp main.cpp -o wiscan.so -lncursesw -lcurses -lncurses -lpthread 
