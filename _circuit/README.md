## FBII XL-2T Alarm Interface Circuit

The alarm system keypad connection consists of 4 wires – ground, data, clock, and +15VDC.  I wanted to be able to use a direct connection from the keypad terminals for the interface.  Using an Arduno is a natural choice since it can handle this voltage level.

For the data and clock signals, I used LM7301 rail-to-rail type op amps for signal interfacing.  The LM7301 op amps can also be directly connected to +15VDC.  The op amp outputs go through a voltage divider to limit the signal voltages to the Arduino inputs.

To pull the data signal low (to send keypress data), I used a small-signal type NPN transistor on the data line.  Sending a high level to the NPN base turns the transistor on, which sends the data signal to ground.

I considered using optoisolators for signal interfacing, but the alarm system is powered by a low-voltage transformer which isolates it from the AC line.  Also, the connection diagram indicates that the power for the keypad is power-limited (500 mA max), so I feel that using op amps which I am already familiar with should be fine.

I would eventually like to make a permanent circuit using SMT components and housing the interface in an enclosure of some sort.  I wouldn’t need a display (would instead rely on the GUI for this purpose), so I might also be able to use a smaller microprocessor board.

5/15/2020

For the final circuit I think I will use a "proper" level converter chip to get the signals from 12V to 5V.  Maybe a MAX232.  I used the LM7301 because that's what I have readily available.
