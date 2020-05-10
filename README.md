## FBII XL-2T Alarm Interface

### Goals

The main goal of this project is to develop an interface for my FBII XL-2T home alarm system (using the keypad connections) so I can remotely monitor and control my alarm system.

My ultimate goal is to have a web-based GUI interface.  Another desired feature is to make the system programming much simpler by use of a GUI-type method (instead of the step-through coded lights it currently uses), and to be able to store and recall programmed settings.

I already figured out how to get alarm status values, and send keypresses along the data line.  My next step is to figure out how to make a GUI-type interface for web/remote access.

### Background

I installed my home security system as a DIY project 20+ years ago.  It is a hardwired FBII (Fire Burglary Instruments Inc.) XL-2T system.  The only interface to the system for programming and control is via the LED keypad.  I’ve heard there is an option to connect a PC to the system for programming purposes, but I do not see any connections labelled as such (perhaps a telephone modem-type device needs to be used for this).  The pics folder contains pictures of my alarm panel and keypad.  I think FBII has since been acquired by Honeywell.

Programming the system is done with the keypad, but the process is very difficult and time-consuming.  It basically involves stepping through a program list sequentially, and using the LED lights to determine the program step and data values.  The LEDs are displayed in a binary-coded fashion (there is no LCD screen) so you have to manually translate the patterns into numbers.

I have always wanted to be able to monitor and control my system remotely, but lacked the means/know-how to do so.  I recently started to tinker around with microprocessors and programming (mainly in C) and decided that trying to make an alarm interface might be a good way to apply this to practical use.

My microprocessor of choice is the Arduino.  I have also messed around with Raspberry Pi’s, but have not really gotten familiar with programming for that platform.  For this project I am thinking of using the Arduino for the alarm system interface, and using an RPi for the network/GUI interface.  Connection between the RPi and Arduino would be via a serial (USB?) interface.

### Data & Clock Signals

Data packets consist of 32 bits sent every 19.5 ms on the signal line.  Alarm system status bits are during the high clock levels. while keypad button press bits are during the low clock levels.  A minimum of two identical keypad packets need to be sent in order to be interpreted as a valid keypress.  See the data signal folder for more info.
