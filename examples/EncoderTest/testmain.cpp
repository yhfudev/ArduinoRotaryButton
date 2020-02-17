

#if defined(ARDUINO)
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <SPI.h>
#include <Wire.h>
//#include <SoftwareSerial.h>
#else
#include <unistd.h>
#define delay(a) usleep(a)
#endif

#include "RotaryButton.h"
#include "Encoder.h"

////////////////////////////////////////////////////////////////////////////////
#if defined(ARDUINO_ARCH_ESP8266)
// ESP8266
#define PIN_ROTARY_BTN D3
#define PIN_ROTARY_A   D5
#define PIN_ROTARY_B   D7

#elif defined(ARDUINO_ARCH_ESP32)
// ESP32
#define PIN_ROTARY_BTN 34
#define PIN_ROTARY_A   35
#define PIN_ROTARY_B   39

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

static Encoder e = Encoder(PIN_ROTARY_A, PIN_ROTARY_B, PIN_ROTARY_BTN);

#define LV_INDEV_STATE_PR  1
#define LV_INDEV_STATE_REL 2

void show_msg()
{
  //Serial.println("my_encoder_read is called by lv.");
  int state;

  int enc_diff = e.get_diff();

  if(e.isPressed()) state = LV_INDEV_STATE_PR;
  else state = LV_INDEV_STATE_REL;

  Serial.print("diff="); Serial.println(enc_diff);
  Serial.print("state="); Serial.println(state);

}

void setup()
{
#if defined(ARDUINO)
    Serial.begin(9600);
    // Wait for USB Serial.
    while (!Serial) {}
    delay(200);
    // Read any input
    while (Serial.read() >= 0) {}
#endif

}

static int pre_disp = 0;

void loop()
{
  int cur;
  e.loopOnce();
  cur = millis();
  if (pre_disp + 100 < cur) {
    pre_disp = cur;
    show_msg();
  }
}

