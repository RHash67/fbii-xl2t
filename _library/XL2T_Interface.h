/*
XL2T_Interface.h - Library for an FBII (Honeywell) XL-2T alarm panel
interface.  Uses the keypad wiring to monitor alarm status and send
keypad presses.  Developed for the Arduino series microprocessor boards.
Caution: the alarm panel clock and data signals are at +12V-+15V
levels - need to drop down to appropriate levels for interfacing
with the Arduino.

Created by Ryan Hashiro, May 2020.
*/

#ifndef XL2T_Interface_h
#define XL2T_Interface_h

#include "Arduino.h"
#include <inttypes.h>

// Default Arduino pin assignments
#define CLKPIN A1
#define DATAPIN A0
#define SHUNTPIN A2

// Keypad integer assignments
#define STAR 10
#define NUM 11
#define STAY 12
#define INSTANT 13
#define BYPASS 14
#define CODE 15

// Keypad signal data
#define KEYPAD_0 0xFFFEFFFF
#define KEYPAD_1 0xBC7FEFFF
#define KEYPAD_2 0x7BFFDFFF
#define KEYPAD_3 0xF7FFFF7F
#define KEYPAD_4 0xEFFFFBFF
#define KEYPAD_5 0xFFFFFEFF
#define KEYPAD_6 0xFFFF7FFF
#define KEYPAD_7 0xDFBFF7FF
#define KEYPAD_8 0xFFFFFFBF
#define KEYPAD_9 0xFFFFFDFF
#define KEYPAD_STAR 0xFFDFFFDF
#define KEYPAD_NUM 0xFFEFFFEF
#define KEYPAD_STAY 0xFFFFBFFF
#define KEYPAD_INSTANT 0xFFFDFFFF
#define KEYPAD_BYPASS 0xFFFBFFFE
#define KEYPAD_CODE 0xFFF7FFF1
#define KEYPAD_NULL 0xFFFFFFFF

class XL2T_Interface
{
  public:
    XL2T_Interface();
    XL2T_Interface(uint8_t clkpin, uint8_t datapin, uint8_t shuntpin);
    uint32_t readAllStatus();
    uint16_t readStatus();
    uint8_t readBuzzer();
    uint8_t readReadyLED();
    uint8_t readInstantLED();
    uint8_t readStayLED();
    uint8_t readArmLED();
    uint8_t readACLED();
    uint8_t readZoneLED(uint8_t zone);
    void sendKey(uint8_t key);
  
  private:
    uint8_t _clkpin;
    uint8_t _datapin;
    uint8_t _shuntpin;
    uint8_t _zone;
    uint8_t _key;
    uint32_t _datapacket;
    uint32_t _pulsezerolen;
    uint8_t _pulsecount;
    uint8_t _alarmarray[34];
    uint8_t _pulselvl;
    uint32_t _tmptime;
    uint8_t _signallvl;
    uint32_t _keybitmask;
    uint32_t _keypadstat;
    uint32_t _tmppacket;
    uint16_t _tmppacket16;
};

#endif
