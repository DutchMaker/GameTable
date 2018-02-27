#ifndef display_h
#define display_h

#include "Arduino.h"
#include <FastLED.h>

#define DISPLAY_PIN_LEDS    9
#define DISPLAY_BRIGHTNESS  64
#define DISPLAY_COLOR_ORDER GRB
#define DISPLAY_CHIPSET     WS2811
#define DISPLAY_FPS         30

class Display
{
  public:
    void    setup();
    void    update();
    void    set_pixel(byte x, byte y, byte value);
    void    clear_pixel(byte x, byte y);
    uint8_t get_pixel(byte x, byte y);
    void    clear_pixels();
    
  private:
    void shiftout(byte* data);
    void spi_send(byte data);
    void clearscreen();
    byte pcb_rev1_fix_x(byte x);
    byte pcb_rev1_fix_y(byte y);

    unsigned long _last_update;
    bool _cleared;
    
    byte _data[9];
    byte _framebuffer[20][10];
    byte _palette[8][3] = { { 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 0 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 1 } };
};

#endif