Arduino library files for the XL2T keypad interface.

5/25/2020

Modified the library cpp file to add a test for the micros() rollover.  Basically resets the test for start of packet detection if the time duration calculation goes out-of-scale.

6/2/2020

Changed the function for the read data packet in the cpp file to load the return variable while the packet is being read (rather than using an array of separate values and then assembling the value after the read).  The code is a bit cleaner, and saves a bit on RAM.
