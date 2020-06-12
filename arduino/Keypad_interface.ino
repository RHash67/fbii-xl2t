// Sketch for interfacing an FBII XL-2T keypad connection to an RPi for
// a web-based monitoring and control.
// If individual byte characters ASCII 48 to 63 are received from the RPi,
// this sends a corresponding keypress (offset to start at 0 value) to the
// XL-2T. A byte character ASCII 64 ("@") received from the RPi will cause a
// 2-byte status data packet to be read and sent to the RPi.

#include "Arduino.h"
#include "XL2T_Interface.h"
#include <SoftwareSerial.h>
#include <inttypes.h>

SoftwareSerial mySerial(11, 10);  // RX, TX
// XL2T_Interface XL2T(4, 5, 6); // clock, data, shunt
XL2T_Interface XL2T(7, 6, 5);


uint8_t incomingByte = 0;
uint16_t data16 = 0;

void setup() {
  mySerial.begin(38400);
}

void loop() {

  while (!mySerial.available()) {}  // wait until data is received
  
  incomingByte = mySerial.read();

  if (incomingByte == 64) {  // '@' = return alarm status data packet
    mySerial.print(XL2T.readStatus());
  }

  if (incomingByte != 64) {
    XL2T.sendKey(incomingByte-48);  // or send keypress
  }

}
