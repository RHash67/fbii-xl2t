/*
XL2T_Interface.cpp - Library for an FBII (Honeywell) XL-2T alarm panel
interface.  Uses the keypad wiring to monitor alarm status and send
keypad presses.  Developed for the Arduino series microprocessor boards.
Caution: the alarm panel clock and data signals are at +12V-+15V
levels - need to drop down to appropriate levels for interfacing
with the Arduino.

Created by Ryan Hashiro, May 2020.

Communication between the keypad and alarm panel is done over the
clock (green wire) and data (yellow wire) lines.  Clock signal for a
data packet consists of 33 pulses - a start pulse and 32 sync pulses.
Period for one pulse is 500 us.  End of packet data to beginning of
next packet data is about 3300 us.

Alarm status is read on the data line during the high levels of the
clock.  Keypress signals are sent during the low levels of the clock,
by sending ("shunting") the data line to ground during the low clock level
at certain pulses.

This library assumes NPN type transistors are used as the interface
and switching means, so reversed states are used (e.g. when the clock
or data lines are high, the corresponding digital input levels are low).

Individual bits in the alarm status signal indicate the state of the
keypad buzzer and LEDs.  These bits occur within the 1st 2 bytes of
the status value.  This library includes routines for reading the
entire status value (32 bits), as well as obtaining individual LED/buzzer
status.
*/

#include "Arduino.h"
#include "XL2T_Interface.h"
#include <inttypes.h>

XL2T_Interface::XL2T_Interface()
{ // set default values for pins.  A1=CLKPIN, A0=DATAPIN, A2=SHUNTPIN
  _clkpin = CLKPIN;
  _datapin = DATAPIN;
  _shuntpin = SHUNTPIN;
  pinMode(_clkpin, INPUT_PULLUP);
  pinMode(_datapin, INPUT_PULLUP);
  pinMode(_shuntpin, OUTPUT);
  digitalWrite(_shuntpin, LOW);
}

XL2T_Interface::XL2T_Interface(uint8_t clkpin, uint8_t datapin, uint8_t shuntpin)
{  // allow user-defined pins
  _clkpin = clkpin;
  _datapin = datapin;
  _shuntpin = shuntpin;
  pinMode(_clkpin, INPUT_PULLUP);
  pinMode(_datapin, INPUT_PULLUP);
  pinMode(_shuntpin, OUTPUT);
  digitalWrite(_shuntpin, LOW);
}

uint32_t XL2T_Interface::readAllStatus()
{  // returns entire 32-bit alarm status value.  The state of the keypad
   // indicators occur within the 1st 2 bytes.
  _datapacket = 0;
  _pulsezerolen = 0;  // time of pulse length at 0 clock level
  _pulsecount = 0;  // counter for number of pulses
  
  _pulselvl = digitalRead(_clkpin);
  
  while (_pulselvl == 0) {   // wait for low clock pulse (remember this is reversed signal)
    _pulselvl = digitalRead(_clkpin);
  }
  _tmptime = micros();  // temporarily record current time
  
  while (_pulsezerolen <= 400) {  // wait for a low clock pulse level of > 400 us
    _pulselvl = digitalRead(_clkpin);  // this means we are before start of a data packet
    if (_pulselvl == 1) {
      _pulsezerolen = micros() - _tmptime;
      if (_pulsezerolen > 3300) {
        _pulsezerolen = 0;  // handler for micros() rollover.  Basically throw away packet if rollover occurs
      }
    }
    else {
      _pulsezerolen = 0;
      while (_pulselvl == 0) {   // wait for low clock pulse again if within the data packet waveform
        _pulselvl = digitalRead(_clkpin);
      }
      _tmptime = micros();
    }
  }

  while (_pulsecount != 33) {
    while (_pulselvl == 1) {   // wait for positive clock pulse
      _pulselvl = digitalRead(_clkpin);  // the start pulse occurs while _pulsecount == 0
    }

    _pulsecount += 1;

    for (int x = 0; x < 4; x++) {  // just wasting some time for signal read after clock pulse edge
      int y = x;                  // maybe about 30 us or so?
    }
    
    _signallvl = digitalRead(_datapin);
    _signallvl = _signallvl ^ 1;  // bitwise xor to invert value
    if (_signallvl == 1) {  // add bit from left end since LSB gets sent first
      _bitadd = 0x80000000;
    }
    else {
      _bitadd = 0;
    }
    _datapacket = _datapacket >> 1;
    _datapacket = _datapacket + _bitadd;

    while (_pulselvl == 0) {   // wait for low clock pulse
      _pulselvl = digitalRead(_clkpin);
    }
  }

  return _datapacket;  // returns full (32-bit) alarm status data
}

uint16_t XL2T_Interface::readStatus()
{  // returns only the 1st 2 bytes of the full alarm status
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket >> 16;
  return (uint16_t) _tmppacket;
}

uint8_t XL2T_Interface::readBuzzer()
{  // returns the state of the keypad buzzer from the next packet.  0 = buzzer off, 1 = buzzer on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x80000000;  // buzzer bit is the MSB of the status value.  Mask it out
  if (_tmppacket == 0) {
    return 1;  // if buzzer bit is low, it actually means the buzzer is on.  Opposite from the LED status.
  }
  else {
    return 0;
  }
}

uint8_t XL2T_Interface::readReadyLED()
{  // returns the state of the READY LED.  0 = off, 1 = on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x40000000;  // READY LED is next bit from MSB (bit 30), where MSB = bit 31 and LSB = bit 0
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

uint8_t XL2T_Interface::readInstantLED()
{  // returns the state of the INSTANT LED.  0 = off, 1 = on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x20000000;  // INSTANT LED is bit 29
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

uint8_t XL2T_Interface::readStayLED()
{  // returns the state of the STAY LED.  0 = off, 1 = on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x10000000;  // STAY LED is bit 28
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

uint8_t XL2T_Interface::readArmLED()
{  // returns the state of the ARM LED.  0 = off, 1 = on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x08000000;  // ARM LED is bit 27
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

uint8_t XL2T_Interface::readACLED()
{  // returns the state of the AC/LB LED.  0 = off, 1 = on
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & 0x02000000;  // AC/LB LED is bit 25
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

uint8_t XL2T_Interface::readZoneLED(uint8_t zone)
{  // returns the state of the Zone LED.  Argument = the zone number.
  _zone = zone;
  if ((_zone < 1) || (_zone > 6)) {
    _zone = 1;  // set zone number to 1 if not valid.  System only has 6 zones.
  }
  uint32_t bitmask = 0x01000000 >> (6 - _zone);  // shift mask according to zone number.  Zones 6 to 1 = bits 24 to 19
  _tmppacket = readAllStatus();
  _tmppacket = _tmppacket & bitmask;
  if (_tmppacket == 0) {
    return 0;  // LED bit low means the LED is off.
  }
  else {
    return 1;
  }
}

/*
Valid keypress consists of identical keypress signals sent over a minimum of 2 consecutive data
packets.
*/

void XL2T_Interface::sendKey(uint8_t key)
{
  _key = key;
  _keypadstat = KEYPAD_NULL;
  
  switch(_key) {
    case 0:
      _keypadstat = KEYPAD_0;
      break;
    case 1:
      _keypadstat = KEYPAD_1;
      break;
    case 2:
      _keypadstat = KEYPAD_2;
      break;
    case 3:
      _keypadstat = KEYPAD_3;
      break;
    case 4:
      _keypadstat = KEYPAD_4;
      break;
    case 5:
      _keypadstat = KEYPAD_5;
      break;
    case 6:
      _keypadstat = KEYPAD_6;
      break;
    case 7:
      _keypadstat = KEYPAD_7;
      break;
    case 8:
      _keypadstat = KEYPAD_8;
      break;
    case 9:
      _keypadstat = KEYPAD_9;
      break;
    case STAR:
      _keypadstat = KEYPAD_STAR;
      break;
    case NUM:
      _keypadstat = KEYPAD_NUM;
      break;
    case STAY:
      _keypadstat = KEYPAD_STAY;
      break;
    case INSTANT:
      _keypadstat = KEYPAD_INSTANT;
      break;
    case BYPASS:
      _keypadstat = KEYPAD_BYPASS;
      break;
    case CODE:
      _keypadstat = KEYPAD_CODE;
      break;
    default:
      _keypadstat = KEYPAD_NULL;  // default value = no key press
      break;
  }


 for (int i = 1; i <= 2; i++)  {  // need to send minimum 2 keypress packets for valid transmission
    _keybitmask = 0x00000001;  // LSB gets sent 1st, so need to mask from right end
    _pulsezerolen = 0;
    _pulsecount = 0;
    
    _pulselvl = digitalRead(_clkpin);
    while (_pulselvl == 0) {   // wait for low clock pulse
      _pulselvl = digitalRead(_clkpin);
    }
    _tmptime = micros();  // temporarily record current time
  
    while (_pulsezerolen <= 400) {  // wait for a low clock pulse level of > 400 us
      _pulselvl = digitalRead(_clkpin);  // this means we are before start of data packets
      if (_pulselvl == 1) {
        _pulsezerolen = micros() - _tmptime;
        if (_pulsezerolen > 3300) {
          _pulsezerolen = 0;  // handler for micros() rollover.  Basically throw away packet if rollover occurs
        }
      }
      else {
        _pulsezerolen = 0;
        while (_pulselvl == 0) {   // wait for low clock pulse again if within the data packet waveform
          _pulselvl = digitalRead(_clkpin);
        }
        _tmptime = micros();
      }
    }

    while (_pulsecount != 32) {
      _pulselvl = digitalRead(_clkpin);
      while (_pulselvl == 1) {   // wait for positive clock pulse
        _pulselvl = digitalRead(_clkpin);
      }
      digitalWrite(_shuntpin, LOW);  // ensure signal is not pulled to ground
      _pulsecount += 1;

      while (_pulselvl == 0) {   // wait for low clock pulse
        _pulselvl = digitalRead(_clkpin);
      }
    
      if ((_keypadstat & _keybitmask) == 0) {
        digitalWrite(_shuntpin, HIGH);  // turns on transistor so signal is pulled to ground
      }     // signal should already be at high level if not so no need to change if != 0

      _keybitmask = _keybitmask << 1;  // shift left to mask next bit sent
    }

    digitalWrite(_shuntpin, LOW);
  }
}


  