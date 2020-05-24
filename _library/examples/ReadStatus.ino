/*
Simple sketch which returns the 32-bit (4-byte) and 16-bit (2-byte) system status
values of the kaypad interface.  The 16-bit value is the 1st 2 bytes of the 32-bit value.
During interface testing only the 1st 2 bytes seem to be affected by alarm events, so the
16-bit value should be sufficient for monitoring system status.
*/

#include "XL2T_Interface.h"
#include "Arduino.h"
#include <inttypes.h>

XL2T_Interface XL2T;  /* creates a class instance XL2T with default pin assignments.
Default is Pin A1: Clock, Pin A0: Data, and Pin A2: Shunt.
To create a class instance with custom pin assignments, include the pins in
parentheses.  e.g.  XL2T_Interface XL2T(4, 5, 6); assigns Pin 4: Clock, Pin 5: Data,
and Pin 6: Shunt.
*/
uint16_t data16 = 0;
uint32_t data32 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  data32 = XL2T.readAllStatus();  // returns entire 32-bit alarm system status value
  data16 = XL2T.readStatus();  // returns the 16-bit value
  Serial.print(data32, HEX);
  Serial.print("  ");
  Serial.println(data16, HEX);
  delay(2000);
}
