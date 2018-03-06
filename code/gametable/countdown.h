#ifndef countdown_h
#define countdown_h

#include "Arduino.h"
#include "display.h"
#include "numericdisplay.h"

class Countdown
{
  public:
    void setup(Display* display, NumericDisplay* numeric_displays);
    void reset();
    void update();

    bool finished;

  private:
    Display*        _display;
    NumericDisplay* _numeric_displays;
    
    int8_t        _countdown_state;
    unsigned long _last_update; 
    bool          _text_displayed;
};

#endif