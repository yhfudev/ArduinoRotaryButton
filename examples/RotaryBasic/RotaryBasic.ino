/**
 * \file    RotaryBasic.ino
 * \brief   rotary encoder simple test
 */
////////////////////////////////////////////////////////////////////////////////
#include "Arduino.h"
#include "RotaryButton.h"

////////////////////////////////////////////////////////////////////////////////
#if defined(ARDUINO_ARCH_ESP8266)
// ESP8266
#define PIN_ROTARY_BTN D6
#define PIN_ROTARY_A   D5
#define PIN_ROTARY_B   D7

#elif defined(ARDUINO_ARCH_ESP32)
// ESP32
#define PIN_ROTARY_BTN 0
#define PIN_ROTARY_A   25
#define PIN_ROTARY_B   26

#elif defined(__AVR_ATmega2560__)
// 2560, 12864 LCD, RepRapDiscount FULL GRAPHIC Smart Controller
#define PIN_ROTARY_BTN 35
#define PIN_ROTARY_A   31
#define PIN_ROTARY_B   33

#elif defined(__AVR_ATmega328P__)
// Arduino Uno
#define PIN_ROTARY_BTN A0
#define PIN_ROTARY_A   A1
#define PIN_ROTARY_B   A2

#elif defined (__AVR_ATmega32U4__)
// ATMega32U4
#define PIN_ROTARY_BTN A0
#define PIN_ROTARY_A   A1
#define PIN_ROTARY_B   A2

#endif

////////////////////////////////////////////////////////////////////////////////
Rotary r = Rotary(PIN_ROTARY_A, PIN_ROTARY_B);

void cb_rotary_position(void * user_data, Rotary::Direction change_type, int offset)
{
  Rotary *pr = (Rotary *)user_data;

  switch (change_type) {
  case Rotary::Direction::Left:
    Serial.print("Rotary Left:   "); Serial.println(offset);
    break;
  case Rotary::Direction::Right:
    Serial.print("Rotary Right:  "); Serial.println(offset);
    break;
  default:
    Serial.print("Rotary (None): "); Serial.println(offset);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("\n\nSimple Rotary Demo");

  r.setHandler(&r, cb_rotary_position);
  r.begin();
}

void loop() {
  r.loopOnce();
}

////////////////////////////////////////////////////////////////////////////////

