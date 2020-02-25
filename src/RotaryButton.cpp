/**
 * \file    RotaryButton.cpp
 * \brief   rotary encoder and button library for Arduino/ESP
 * \author  Yunhui Fu <yhfudev@gmail.com>
 * \version 1.0
 * \date    2020-02-03
 * \copyright GPL/BSD
 */
////////////////////////////////////////////////////////////////////////////////
#include "RotaryButton.h"

#if defined(ARDUINO)

////////////////////////////////////////////////////////////////////////////////
Rotary::Rotary(int pin1, int pin2) {
  this->pin1 = pin1;
  this->pin2 = pin2;

  this->position = 0;
  this->last_read_ms = 0;
}

void Rotary::begin() {
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
}

#define MOVES_PER_TICK 2
void Rotary::loopOnce() {
    int s = this->state & 3;
    if (digitalRead(this->pin1)) s |= 4;
    if (digitalRead(this->pin2)) s |= 8;

    Rotary::Direction direction = Rotary::Direction::None;
    switch (s) {
      case 0: case 5: case 10: case 15:
        break;
      case 1: case 7: case 8: case 14:
        position++; break;
      case 2: case 4: case 11: case 13:
        position--; break;
      case 3: case 12:
        position += 2; break;
      default:
        position -= 2; break;
    }
    this->state = (s >> 2);

    if (position >= MOVES_PER_TICK) {
      direction = Rotary::Direction::Right;
      if (change_cb != nullptr) change_cb (userdata, direction, position/MOVES_PER_TICK);
      position -= MOVES_PER_TICK;
    } else if (position <= - MOVES_PER_TICK) {
      direction = Rotary::Direction::Left;
      if (change_cb != nullptr) change_cb (userdata, direction, position/MOVES_PER_TICK);
      position += MOVES_PER_TICK;
    }
}

////////////////////////////////////////////////////////////////////////////////
void Button::loopOnce() {
  prev_state = state;
  state = digitalRead(pin);

  // is button pressed?
  if (prev_state == HIGH && state == LOW) {
    down_ms = millis();
    pressed_triggered = false;
    click_count++;
    click_ms = down_ms;

  // is the button released?
  } else if (prev_state == LOW && state == HIGH) {
    down_time_ms = millis() - down_ms;
    // is it beyond debounce time?
    if (down_time_ms >= debounce_time_ms) {
      if (change_cb != NULL) {
        this->change_cb (this->userdata, Button::PressEvent::Changed);
        // trigger release
        this->change_cb (this->userdata, Button::PressEvent::Released);
        // trigger tap
        this->change_cb (this->userdata, Button::PressEvent::Tap);
      }
      // was it a longclick? (preceeds single / double / triple clicks)
      if (down_time_ms >= LONGCLICK_MS) {
        longclick_detected = true;
      }
    }

  // trigger pressed event (after debounce has passed)
  } else if (state == LOW && !pressed_triggered && (millis() - down_ms >= debounce_time_ms)) {
    if (change_cb != NULL) {
      this->change_cb (this->userdata, Button::PressEvent::Changed);
      this->change_cb (this->userdata, Button::PressEvent::Pressed);
    }
    pressed_triggered = true;

  // is the button pressed and the time has passed for multiple clicks?
  } else if (state == HIGH && millis() - click_ms > DOUBLECLICK_MS) {
    // was there a longclick?
    if (longclick_detected) {
      // was it part of a combination?
      if (click_count == 1) {
        last_click_type = LONG_CLICK;
        if (change_cb != NULL) {
          this->change_cb (this->userdata, Button::PressEvent::Long);
        }
      }
      longclick_detected = false;
    // determine the number of single clicks
    } else if (click_count > 0) {
      switch (click_count) {
        case 1:
          last_click_type = SINGLE_CLICK;
          if (change_cb != NULL) {
            this->change_cb (this->userdata, Button::PressEvent::Click);
          }
          break;
         case 2:
          last_click_type = DOUBLE_CLICK;
          if (change_cb != NULL) {
            this->change_cb (this->userdata, Button::PressEvent::Double);
          }
          break;
         case 3:
          last_click_type = TRIPLE_CLICK;
          if (change_cb != NULL) {
            this->change_cb (this->userdata, Button::PressEvent::Triple);
          }
          break;
      }
    }
    click_count = 0;
    click_ms = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
#endif // ARDUINO

