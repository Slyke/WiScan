#!/bin/bash

# Replace with makefile

# rm wiscan

g++ -lncurses -lpthread cli.cpp wifilist.cpp wificell.cpp main.cpp -o wiscan
