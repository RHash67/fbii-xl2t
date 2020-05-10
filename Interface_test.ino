// This sketch tests communication with an FBII XL-2T alarm panel via the keypad connection
//
// The hex value of the alarm status and the keypad button being sent is displayed.
// The sketch cycles through all keypad buttons. If a valid button press is sent a beep
// should be heard from the keypad (MSB in the alarm status value driven low).
//
// Data signal consists of 1 start bit and 32 data bits per packet.
//
// NOTE:  Minimum of 2 identical data packets need to be sent from keypad for valid transmission

// library code for LCD display
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

#define CLKPIN      A1
#define DATAPIN   A0
#define OUTPUTPIN   A2

#define STAR   10
#define NUM    11
#define STAY   12
#define INSTANT  13
#define BYPASS   14
#define CODE   15

Adafruit_LiquidCrystal lcd(0);

unsigned long getStatus() {  //function to get alarm status value of next packet
  unsigned long datapacket = 0;
  unsigned long pulsezerolen = 0;  // time of pulse length at 0 clock level
  int pulsecount = 0;  // counter for number of pulses
  int alarmarray[34];  // array to hold alarm signal values
  for (int i = 0; i <= 33; i++) {  // not sure if this is needed?
    alarmarray[i] = 0;
  }
  
  int pulselvl = digitalRead(CLKPIN);
  
  while (pulselvl == 1) {   // wait for 0 pulse
    pulselvl = digitalRead(CLKPIN);
  }
  unsigned long tmp = micros();  // temporarily record current time
  
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

  while (pulsecount != 33) {
    while (pulselvl == 0) {   // wait for positive pulse
      pulselvl = digitalRead(CLKPIN);  // when pulsecount = 0 is the start pulse
    }

    pulsecount = pulsecount + 1;

    delayMicroseconds(30);  // need a small delay to read signal after clock pulse edge
    int signallvl = digitalRead(DATAPIN);
    alarmarray[pulsecount] = signallvl;  // note:  alarmarray[1] is actually the start pulse.  Can throw away

    while (pulselvl == 1) {   // wait for 0 pulse
      pulselvl = digitalRead(CLKPIN);
    }
  }

    for (int i = 1; i < 34; i++) {
      datapacket = datapacket >> 1;
      if (alarmarray[i] == 1) {
        datapacket = datapacket + 0x80000000;  // load bits from left end, and shift right
      }
    }

  return datapacket;  // returns alarm status data
}

void sendKey(int keydata) {  // sends key data to next packet

  unsigned long keypadstat = 0xFFFFFFFF;
  
  switch(keydata) {
    case 0:
      keypadstat = 0xFFFEFFFF;
      break;
    case 1:
      keypadstat = 0xBC7FEFFF;
      break;
    case 2:
      keypadstat = 0x7BFFDFFF;
      break;
    case 3:
      keypadstat = 0xF7FFFF7F;
      break;
    case 4:
      keypadstat = 0xEFFFFBFF;
      break;
    case 5:
      keypadstat = 0xFFFFFEFF;
      break;
    case 6:
      keypadstat = 0xFFFF7FFF;
      break;
    case 7:
      keypadstat = 0xDFBFF7FF;
      break;
    case 8:
      keypadstat = 0xFFFFFFBF;
      break;
    case 9:
      keypadstat = 0xFFFFFDFF;
      break;
    case STAR:
      keypadstat = 0xFFDFFFDF;
      break;
    case NUM:
      keypadstat = 0xFFEFFFEF;
      break;
    case STAY:
      keypadstat = 0xFFFFBFFF;
      break;
    case INSTANT:
      keypadstat = 0xFFFDFFFF;
      break;
    case BYPASS:
      keypadstat = 0xFFFBFFFE;
      break;
    case CODE:
      keypadstat = 0xFFF7FFF1;
      break;
    default:
      keypadstat = 0xFFFFFFFF;  // default value = no key press
      break;
  }


 for (int i = 1; i <= 2; i++)  {  // need to send minimum 2 keypress packets for valid transmission
    unsigned long keybitmask = 0x00000001;  // LSB gets sent 1st, so need to mask from right end
    int pulsezerolen = 0;
    int pulsecount = 0;
    
    int pulselvl = digitalRead(CLKPIN);
    while (pulselvl == 1) {   // wait for 0 pulse
      pulselvl = digitalRead(CLKPIN);
    }
    unsigned long tmp = micros();  // temporarily record current time
  
    while (pulsezerolen <= 400) {  // wait for a 0 pulse level of > 400 us
      pulselvl = digitalRead(CLKPIN);  // this means we are before start of data packets
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

    while (pulsecount != 32) {
      pulselvl = digitalRead(CLKPIN);
      while (pulselvl == 0) {   // wait for positive pulse
        pulselvl = digitalRead(CLKPIN);
      }
      digitalWrite(OUTPUTPIN, LOW);  // ensure signal is not pulled to ground
      pulsecount = pulsecount + 1;

      while (pulselvl == 1) {   // wait for 0 pulse
        pulselvl = digitalRead(CLKPIN);
      }
    
      if ((keypadstat & keybitmask) == 0) {
        digitalWrite(OUTPUTPIN, HIGH);  // turns on transistor so signal is pulled to ground
      }     // signal should already be at high level if not so no need to change if != 0

      keybitmask = keybitmask << 1;  // shift left to mask next bit sent
    }

    digitalWrite(OUTPUTPIN, LOW);
  }
}


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  lcd.setBacklight(HIGH);
 
  pinMode(CLKPIN, INPUT_PULLUP);
  pinMode(DATAPIN, INPUT_PULLUP);
  pinMode(OUTPUTPIN, OUTPUT);
  digitalWrite(OUTPUTPIN, LOW);

}

void loop() {
  for (int i = 0; i <= 15; i++) {
    delay(2000);
    unsigned long alarmstat = getStatus();
    lcd.setCursor(0, 0);
    lcd.print(alarmstat, HEX);
    lcd.print("  ");
    lcd.print(i);
    sendKey(i);
    alarmstat = getStatus();
    lcd.print("  ");
    lcd.print(alarmstat, HEX);
  }

}
