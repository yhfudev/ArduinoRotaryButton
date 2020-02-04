/**
 * \file    RotaryButton.h
 * \brief   rotary encoder and button library for Arduino/ESP
 * \author  Yunhui Fu <yhfudev@gmail.com>
 * \version 1.0
 * \date    2020-02-03
 * \copyright GPL/BSD
 */
#ifndef ROTARY_BUTTON_H
#define ROTARY_BUTTON_H 1

////////////////////////////////////////////////////////////////////////////////
class Rotary {
 public:
    typedef enum { None, Left, Right } Direction;
    typedef void (*Callback) (void * user_data, Rotary::Direction change_type, int offset);

    Rotary(int pin1, int pin2);
    void setHandler(void * user_data, Rotary::Callback f) {userdata = user_data; change_cb = f;}
    void loopOnce();

 private:
    int pin1, pin2;
    int position;
    unsigned long last_read_ms;
    byte state;

    Rotary::Callback change_cb = nullptr;
    void *userdata = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
#define DEBOUNCE_MS      50
#define LONGCLICK_MS    250
#define DOUBLECLICK_MS  400

#define SINGLE_CLICK      1
#define DOUBLE_CLICK      2
#define TRIPLE_CLICK      3
#define LONG_CLICK        4

class Button {
  public:
    typedef enum { None, Pressed, Released, Changed, Click, Tap, Long, Double, Triple } PressEvent;
    typedef void (*Callback) (void * user_data, Button::PressEvent type);

    Button(byte attachTo, byte buttonMode = INPUT_PULLUP, unsigned int debounceTimeout = DEBOUNCE_MS)
      : pin(attachTo) {
         setDebounceTime(debounceTimeout);
         pinMode(attachTo, buttonMode);
      }
    void setDebounceTime(unsigned int ms) {debounce_time_ms = ms;}

    void setHandler(void * user_data, Button::Callback f) {userdata = user_data; change_cb = f;}

    unsigned int wasPressedFor() {return down_time_ms;}
    boolean isPressed() {return (state == LOW);}

    unsigned int getNumberOfClicks() {return click_count;}
    unsigned int getClickType() {return last_click_type;}

    bool operator==(Button &rhs) {return (this==&rhs);}

    void loopOnce();

  private:
    byte pin;
    int prev_state;
    int state = HIGH;
    byte click_count = 0;
    unsigned int last_click_type = 0;
    unsigned long click_ms;
    unsigned long down_ms;
    unsigned int debounce_time_ms;
    unsigned int down_time_ms = 0;
    bool pressed_triggered = false;
    bool longclick_detected = false;

    Button::Callback change_cb = nullptr;
    void *userdata = nullptr;

};

////////////////////////////////////////////////////////////////////////////////
#endif // ROTARY_BUTTON_H
