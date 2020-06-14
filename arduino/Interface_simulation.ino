// Simulates the arduino alarm interface and circuit.  Useful if you want to test
// the RPi serial connection and web interface without having to connect the actual
// alarm system.  Just connect the arduino serial tx and rx to the RPi tx/rx.  The RPi
// uses 3.3V logic, so either use a 3.3V logic board or use a logic level converter.
//
// This simulation will cycle through various alarm status values every 2 seconds.  The
// RPi polls the interface for status once every second so all the indicators should
// be visible.  The 1st status value is the normal ready and unarmed mode of the alarm
// (AC/LB and READY LEDs lit).  Then, each LED will cycle on individually.  After that,
// a simulated alarm setoff event on Zone 1 will occur for 10 seconds (this is so you
// can test your email notification alert).  The cycle will then start from the beginning.
//
// For keypresses, the built-in LED on the arduino board will flash the number of times
// of the index value.  For numerals 1 through 9, this is the same as the number itself.
// For 0, the LED will flash 3 times quickly.

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <inttypes.h>

#define NORMAL 0xC207
#define STAT1 0x8807
#define STAT2 0x9007
#define STAT3 0xA007
#define STAT4 0x8207
#define STAT5 0xC007
#define ZONE1 0x800F
#define ZONE2 0x8017
#define ZONE3 0x8027
#define ZONE4 0x8047
#define ZONE5 0x8087
#define ZONE6 0x8107
#define ALARMEVENT 0x080F
#define STDONTIME 170 // value in ms
#define STDOFFTIME 170
#define ZEROONTIME 100
#define ZEROOFFTIME 100
#define STATUSCHANGE 2000
#define ALARMDURATION 10000 // change this to less than 5000 if you don't want email
                          // notifications to be sent on alarm simulation

SoftwareSerial mySerial(9, 10);  // RX, TX (comm to RPi)

uint16_t alarmStatus[13] = {NORMAL, STAT1, STAT2, STAT3, STAT4, STAT5, ZONE1, ZONE2,
  ZONE3, ZONE4, ZONE5, ZONE6, ALARMEVENT};
uint16_t sendStatus = alarmStatus[0];
uint8_t incomingByte = 0;
uint8_t blinkCount = 0;
uint8_t blinkIndex = 0;
uint8_t statusIndex = 0;
uint8_t blinkMode = 0;  // 0 = LED off mode, 1 = LED in blink mode
uint8_t ledState = 0;
uint8_t blinkOntime = STDONTIME;
uint8_t blinkOfftime = STDOFFTIME;
unsigned long statusUpdatetime = STATUSCHANGE;
unsigned long pollTime = millis();
unsigned long blinkChangetime = millis();


void setup() {
  mySerial.begin(38400); // (pin 3 & 4 = Rx, Tx for Trinket M0
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  sendStatus = alarmStatus[statusIndex];
  if ((millis() - pollTime) > statusUpdatetime) {
    statusIndex += 1;
    if (statusIndex > 12) {
      statusIndex = 0;
    }
    if (statusIndex == 12) {
      statusUpdatetime = ALARMDURATION;
    }
    else {
      statusUpdatetime = STATUSCHANGE;
    }
    pollTime = millis();
  }

  if (blinkMode == 1) {  // check LED state and switch on or off as appropriate
    if ((ledState == 1) && ((millis() - blinkChangetime) > blinkOntime)) {
      ledState = 0;
      digitalWrite(LED_BUILTIN, ledState);
      blinkChangetime = millis();
      if (blinkIndex == blinkCount) {  // we have reached the blink counter
        blinkMode = 0;  // turn off blinking mode
      }
    }
    if ((ledState == 0) && ((millis() - blinkChangetime) > blinkOfftime)) {
      ledState = 1;
      digitalWrite(LED_BUILTIN, ledState);
      blinkChangetime = millis();
      blinkIndex += 1;
    }
  }

  if (mySerial.available() != 0) {  // serial data is awaiting read
    incomingByte = mySerial.read();
    if (incomingByte == 64) {  // '@', or send data status
      mySerial.print(sendStatus);
    }
    if (incomingByte != 64) {
      blinkMode = 1;
      ledState = 1;
      digitalWrite(LED_BUILTIN, ledState);
      blinkIndex = 1;
      blinkChangetime = millis();
      blinkCount = incomingByte - 48;
      if (blinkCount == 0) {
        blinkOntime = ZEROONTIME;
        blinkOfftime = ZEROOFFTIME;
        blinkCount = 3;
      }
      else {
        blinkOntime = STDONTIME;
        blinkOfftime = STDOFFTIME;
      }
    }
  }
 
}
