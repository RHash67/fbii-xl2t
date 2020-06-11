## FBII XL-2T Alarm Interface Circuit

The alarm system keypad connection consists of 4 wires – ground, data, clock, and +15VDC.  A 2A switch-mode voltage regulator on the +15VDC line will be used to step the voltage down to 5VDC to feed the Arduino and the Raspberry Pi.  During testing I found that the keypad connector has enough capacity to power an arduino and a RPi Model B via the voltage regulator.  A Raspberry Pi Zero W can also be used - this is what I will be using since it has a built-in wifi interface.  The advantage of using the keypad power to feed the interface is that the system will continue to operate upon loss of AC power via the alarm system's battery.  I am not sure if an RPi 3 or 4 can be used - as I understand it the power consumption of these is a lot greater.

You can use any small-signal type NPN transistors for the circuit.  Make sure the voltage rating is at least 20V.

I decided to use an Adafruit Metro Mini 328 board for the arduino interface, since it has a solder jumper which converts it from 5V logic to 3.3V logic.  Thus the serial connection can be sent directly to the RPi (which uses 3.3V logic), instead of having to use a separate logic level converter chip.  If you use a board with 5V logic, make sure you use a logic level converter on the serial connection to the RPi.

I considered using optoisolators for signal interfacing, but the alarm system is powered by a low-voltage transformer which isolates it from the AC line.  Also, the alarm connection diagram indicates that the power for the keypad is power-limited (500 mA max), so I feel that using a direct connection should be OK.

I experimented with using the Raspberry Pi for both the signal processing and web server functions.  The RPi could capture the signals most of the time, but there were occasional timing errors in the capture.  Also, keypress sends were hit-and-miss.  I think the GPIO functions in the RPi python are not meant for accurate timing/control functions.  I was hoping to be able to just use one device for everything, but as it turns out, it is a lot better to use a dedicated board for the keypad data capture function.
