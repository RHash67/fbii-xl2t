## FBII XL-2T Interface - Arduino sketch

The Keypad_Interface.ino sketch is for the arduino in the circuit diagram in this repository.

Serial communication with the Raspberry Pi is very simple.  A 1-byte character is sent from the RPi for an
interface request.  ASCII characters 48 through 63 correspond to keypress signals.  ASCII character 64 causes
the arduino to send the 2-byte alarm status data packet back to the RPi (parsing of the data packet to get the
individual LED display status is done in the RPi since this only requires 2 bytes of data to be sent serially,
rather than 12 separate bytes).

The libraries for this sketch can be found in the libraries folder in this repository.
