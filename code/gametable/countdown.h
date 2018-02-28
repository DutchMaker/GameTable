#ifndef countdown_h
#define countdown_h

#include "Arduino.h"
#include "display.h"

class Countdown
{
  public:
    void setup(Display* display);
    void reset();
    void update();

    bool finished;

  private:
    Display* _display;
    
    int8_t        _countdown_state;
    unsigned long _last_update; 
};

#endif