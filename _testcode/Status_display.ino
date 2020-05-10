// This sketch detects keypad clock pulses for the FBII XL-2T alarm panel
// and displays the signal value of the alarm status (sent during the high clock levels)
// Data signal consist of 1 start bit and 32 data bits per packet.

// library code for LCD display
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

#define CLKPIN      A1
#define DATAPIN   A0

int pulsecount = 0;   // counter for number of pulses
int pulselvl = 0;   // value of clock pulse read.  0 (low) or 1 (high)
int signallvl = 0;  // value of signal level read.
int alarmarray[34];  // array to hold alarm status values
unsigned long tmp = 0;  // temporary variable
unsigned long pulsezerolen = 0;  // time of pulse length at 0 level
unsigned long alarmstat = 0;  // alarm status packet data.  32 pulses = 4 x 8 bytes

Adafruit_LiquidCrystal lcd(0);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  lcd.setBacklight(HIGH);

  pinMode(CLKPIN, INPUT_PULLUP);
  pinMode(DATAPIN, INPUT_PULLUP);
  
}

void loop() {
  pulselvl = digitalRead(CLKPIN);  // need to find start of data packet
  
  while (pulselvl == 1) {   // wait for clock 0 pulse
    pulselvl = digitalRead(CLKPIN);
  }
  tmp = micros();  // temporarily record current time
  
  while (pulsezerolen <= 400) {  // test for a 0 pulse level of > 400 us
    pulselvl = digitalRead(CLKPIN);  // this means we are before start of data packet
    if (pulselvl == 0) {
      pulsezerolen = micros() - tmp;
    }
    else {
      pulsezerolen = 0;
      while (pulselvl == 1) {   // wait for 0 pulse again if within the data packet waveform
        pulselvl = digitalRead(CLKPIN);
      }
      tmp = micros();
    }
  }

  while (pulsecount != 33) {  // OK we are at start of data packet.
    while (pulselvl == 0) {   // wait for positive pulse
      pulselvl = digitalRead(CLKPIN);  // 1st pulse of data packet is a start pulse
    }

    pulsecount = pulsecount + 1;

    delayMicroseconds(30);  // need a small delay to read signal after clock pulse edge?
    signallvl = digitalRead(DATAPIN);
    alarmarray[pulsecount] = signallvl;

    while (pulselvl == 1) {   // wait for 0 pulse
      pulselvl = digitalRead(CLKPIN);
    }
    
  }

    for (int i = 1; i < 34; i++) {
      alarmstat = alarmstat >> 1;  // load 4-byte variable with array from left end
      if (alarmarray[i] == 1) {    // OK to discard alarmarray[1] which is just the start pulse value
        alarmstat = alarmstat + 0x80000000;
      }

    }

  lcd.setCursor(0, 0);
  lcd.print(alarmstat, HEX);

  pulsecount = 0;
  alarmstat = 0;
  for (int i = 0; i <= 33; i++) {
    alarmarray[i] = 0;
  }

  pulsezerolen = 0;
  
}
