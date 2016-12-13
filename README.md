# WiScan


##Compiling
To compile, you'll need to install ncurses:

`sudo apt-get install libncurses5-dev libncursesw5-dev`


##Configs
To stop screen turning off:

`sudo nano /etc/kbd/config`

Set:

`BLANK_TIME=0`

and

`POWERDOWN_TIME=0`

Ensure they are both set to 0.

Then run:

`sudo /etc/init.d/kbd restart`
