#!/bin/bash
 
# Replace with makefile
 
# rm wiscan.so
 
g++ -std=c++11 cli.cpp wifilist.cpp touchinput.cpp uiobj.cpp wificell.cpp keyboardwatch.cpp screens/scan.cpp screens/screens.cpp screens/options1.cpp screens/cell.cpp main.cpp -o wiscan.so -lncursesw -lcurses -lncurses -lpthread 
