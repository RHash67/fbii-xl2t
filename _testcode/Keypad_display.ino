// This sketch detects keypad clock pulses for the FBII XL-2T alarm panel
// and displays the signal value of the keypad status (sent during the low clock levels)
// Display is only shown if changed from the "no keypress" value of 0xFFFFFFFF
// Data signal consists of 1 start bit and 32 data bits per packet.

// library code for LCD display
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

#define CLKPIN      A1
#define DATAPIN   A0

int pulsecount = 0;   // counter for number of pulses
int pulselvl = 0;   // value of clock pulse read.  0 (low) or 1 (high)
int signallvl = 0;  // value of signal level read.
int keyarray[34];  // array to hold keypad signal values
unsigned long tmp = 0;  // temporary variable
unsigned long ptime = 0;  // time in ms at event record
unsigned long pulsezerolen = 0;  // time of pulse length at 0 level
unsigned long keypadstat = 0;  // keypad status packet data.  32 pulses = 4 x 8 bytes

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
  
  while (pulsezerolen <= 400) {  // wait for a 0 pulse level of > 400 us
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

  while (pulsecount != 33) {  // OK we are at start of data packet
    while (pulselvl == 0) {   // wait for positive pulse
      pulselvl = digitalRead(CLKPIN);  // 1st pulse of data packet is a start pulse
    }

    pulsecount = pulsecount + 1;

    while (pulselvl == 1) {   // wait for 0 pulse
      pulselvl = digitalRead(CLKPIN);
    }

    delayMicroseconds(30);  // need a small delay to read signal after clock pulse edge?
    signallvl = digitalRead(DATAPIN);
    keyarray[pulsecount] = signallvl;
    
  }

  ptime = millis();

    for (int i = 1; i < 34; i++) {
      keypadstat = keypadstat >> 1;  // load 4-byte variable with array from left end
      if (keyarray[i-1] == 1) {      // this is so keyarray[0] value gets discarded
        keypadstat = keypadstat + 0x80000000;
      }
    }

    if (keypadstat != 4294967295) {  // only print value if keypad status is different from no press
        lcd.setCursor(0, 0);
        lcd.print(keypadstat, HEX);
        lcd.print("  ");
        lcd.print(ptime);
        lcd.print("  ");

    }
  
  pulsecount = 0;
  keypadstat = 0;
  for (int i = 0; i <= 33; i++) {
    keyarray[i] = 0;
  }

  pulsezerolen = 0;
  
}
