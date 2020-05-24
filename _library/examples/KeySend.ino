/*
Simple sketch which sends keypresses to the system.
The index value is printed along with the send so you can tell what value is being sent.
For nonnumeric keys, the index values are: *: 10, #: 11, STAY: 12, INSTANT: 13, BYPASS: 14,
and CODE: 15.
*/

#include "XL2T_Interface.h"
#include "Arduino.h"
#include <inttypes.h>

XL2T_Interface XL2T;  // class instance creator

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i <= 15; i++) {
    Serial.println(i);
    XL2T.sendKey(i);  // sends a keypress with index value i
    delay(2000);
  }
}
