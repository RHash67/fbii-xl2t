## FBII XL-2T Alarm Interface Circuit

The alarm system keypad connection consists of 4 wires – ground, data, clock, and +15VDC.  The +15VDC wire will not be used, since I intend to connect the arduino to a Raspberry Pi later for signal communication.  I plan to get 5V power for the arduino via the RPi connection.

You can use any small-signal type NPN transistors for the circuit.  Make sure the voltage rating is at least 20V.

I considered using optoisolators for signal interfacing, but the alarm system is powered by a low-voltage transformer which isolates it from the AC line.  Also, the alarm connection diagram indicates that the power for the keypad is power-limited (500 mA max), so I feel that using a direct connection should be OK.

I would eventually like to make a permanent circuit using SMT components and housing the interface in an enclosure of some sort.  I wouldn’t need a display (would instead rely on a GUI for this purpose), so I might also be able to use a smaller microprocessor board.

5/21/2020

The serial tx/rx pins will be used for communication with a Raspberry Pi.  I have decided that it is easier to develop a web-based server app on a RPi than it is with an Arduino.  The Arduino is better suited for signal processing.  Since the RPi uses 3.3V signal interfacing, I will need to use 3.3V type Arduino and modify the circuit accordingly.

5/23/2020

Modified the circuit and arduino code to use NPN type transistors instead of rail-to-rail op amps.  NPN transistors are less expensive, and easier to connect.
