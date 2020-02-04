/**
 * \file    ButtonBasic.ino
 * \brief   button simple test
 */
////////////////////////////////////////////////////////////////////////////////
#include "Arduino.h"
#include "RotaryButton.h"

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

////////////////////////////////////////////////////////////////////////////////
Button b = Button(PIN_ROTARY_BTN);

void show_clicks(int clicks)
{
  Serial.print("(");
  Serial.print(clicks);
  Serial.print(")");
}

void cb_button_pressed (void * user_data, Button::PressEvent type)
{
  Button *pb = (Button *)user_data;
  switch(type) {
  case Button::PressEvent::None:
    Serial.println("Button: None");
    break;
  case Button::PressEvent::Changed:
    Serial.println("Button: Changed");
    break;
  case Button::PressEvent::Pressed:
    Serial.println("Button: Pressed");
    break;
  case Button::PressEvent::Long:
  {
    unsigned int time = pb->wasPressedFor();
    Serial.print("Button: Long (Reset), ");
    if (time > 1500) {
        Serial.print("a really really long time.");
    } else if (time > 1000) {
        Serial.print("a really long time.");
    } else if (time > 500) {
        Serial.print("a long time.");
    } else {
        //Serial.print("long.");
    }
    Serial.print(" (");
    Serial.print(time);
    Serial.println(" ms)");
  }
    break;
  case Button::PressEvent::Released:
    Serial.println("Button: Released");
    break;
  case Button::PressEvent::Click:
    Serial.print("Button: Click (Enter)");
    show_clicks(pb->getNumberOfClicks());
    Serial.println("");
    break;
  case Button::PressEvent::Tap:
    Serial.println("Button: Tap");
    break;
  case Button::PressEvent::Double:
    Serial.print("Button: Double (Quit)");
    show_clicks(pb->getNumberOfClicks());
    Serial.println("");
    break;
  case Button::PressEvent::Triple:
    Serial.print("Button: Triple (Easter eggs)");
    show_clicks(pb->getNumberOfClicks());
    Serial.println("");
    break;
  default:
    Serial.println("Button: Error State");
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Button Demo");

  b.setHandler(&b, cb_button_pressed);
}

void loop() {
  b.loopOnce();
}
////////////////////////////////////////////////////////////////////////////////
