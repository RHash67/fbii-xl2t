## FBII XL-2T Alarm System Keypad Signals

### Properties

The clock signal (green wire) consists of a packet of 33 pulses, sent every 19.5 ms.  The pulse period is 500 microseconds (us).  The first pulse is a start/sync pulse –the data is sent during the rest of the 32 pulses.  The data (yellow wire) is held normally high.  Alarm system status bits are read during the high clock levels. while keypad button press bits are read during the low clock levels.  For keypresses, a minimum of two identical keypad packets need to be sent in succession order to be interpreted as a valid keypress.

### Alarm Signal Values

The bit values of the various alarm status signals, along with their corresponding hex values, are shown in the “Alarm status values.pdf” file.  I am assuming the first bit sent after the start pulse is the LSB of the data packet (in keeping with standard serial communication protocol).

It appears that the keypad LED lights correspond to individual bits within the packet – so I should be able to monitor the alarm status by masking these bits.

### Keypad Signal Values

The hex values of the keypad button presses are shown in the “Keypad send values.pdf” file.  There is no apparent bit pattern between different key presses.  I suppose the system designers did this for signal security.

A minimum of two identical keypad packets need to be sent in succession order to be interpreted as a valid keypress

### Background

At first I tried using standard communication protocol libraries (eg. RS-232, RS-485, etc) with an Arduino connected to the keypad wires to try to communicate with the system.  I had no success with doing this.  Since the alarm panel has no apparent way of connecting to a PC, I came to the conclusion that some sort of proprietary protocol is being used between the keypad and alarm panel.

My next step was to view the clock & signal lines with an oscilloscope.  The stable clock signal became very apparent by doing this, so I could then get a better idea of the basic data structure.  Unfortunately, I do not have access to a digital recording oscope, so I was not able to identify exactly what was going on during keypresses.  Each keypress only affects a few bits, and only for a couple cycles each.  I could see on the oscope that something was happening, but my oscope trigger could not reliably lock on to the start pulses.  It almost appeared as if the keypresses were affecting random bits!

I became discouraged and lost interest in this for awhile – until I started messing around with C programming and learning how to make my own functions.  I realized that a 16 MHz Arduino is more than fast enough to follow the signal pulses with C code, so I made some simple sketches to record both the alarm and keypad status bits.  Sort of like a customized digital recording oscope!  I could then reliably detect and record the data values.
