#!/bin/bash
 
# Replace with makefile
 
# rm wiscan
 
g++ cli.cpp wifilist.cpp touchinput.cpp uiobj.cpp wificell.cpp screens/scan.cpp main.cpp -o wiscan.so -lncursesw -lcurses -lncurses -lpthread 
