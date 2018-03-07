#ifndef countdown_h
#define countdown_h

#include "Arduino.h"
#include "display.h"
#include "numericdisplay.h"
#include "controller.h"

class Countdown
{
  public:
    void setup(Display* display, NumericDisplay* numeric_displays, Controller* controller);
    void reset();
    void update();

    bool finished;

  private:
    Display*        _display;
    NumericDisplay* _numeric_displays;
    Controller*     _controller;
    
    int8_t        _countdown_state;
    unsigned long _last_update; 
    bool          _text_displayed;
};

#endif