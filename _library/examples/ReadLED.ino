/*
Simple sketch which reads individual keypad LEDs and the buzzer status.
Reads are done on separate data packets, so to read all 11 LEDs and the buzzer
using these functions will take about 0.24 seconds.  It is unlikely that multiple alarm
events will change curing this time, so reading the LEDs separately should be adequate
for typical applications.
If for some reason you need to get the status of the buzzer and all LEDs during one
packet read, then you should use the readStatus() or readAllStatus() function.  You will
need routines to mask the bits out yourself if you do this.
The functions return 0 if the respective LED (or buzzer) is off, and 1 if the LED (or buzzer)
is on.  An alarm event will cause the buzzer to sound continuously, so if the buzzer returns
a 1 for say 5 seconds continously, then that is an indication that the alarm has been activated.
*/

#include "XL2T_Interface.h"
#include "Arduino.h"
#include <inttypes.h>

XL2T_Interface XL2T;

uint8_t ledstatus = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  ledstatus = XL2T.readBuzzer();  // returns the buzzer status
  Serial.print("Buzzer: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readArmLED();  // returns the Arm LED status
  Serial.print("Arm LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readStayLED();  // returns the Stay LED status
  Serial.print("Stay LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readInstantLED();  // returns the Instant LED status
  Serial.print("Instant LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readACLED();  // returns the AC/LB LED status
  Serial.print("AC/LB LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readReadyLED();  // returns the Ready LED status
  Serial.print("Ready LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readZoneLED(1);  // returns the Zone 1 LED status
  Serial.print("Zone 1 LED: ");
  Serial.println(ledstatus);
  ledstatus = XL2T.readZoneLED(2);  // returns the Zone 2 LED status
  Serial.print("Zone 2 LED: ");  // follow same format for Zones 3 - 6 LEDs
  Serial.println(ledstatus);
  delay(2000);
}
