#ifndef display_h
#define display_h

#include "Arduino.h"
#include <FastLED.h>

#define DISPLAY_PIN_LEDS    9
#define DISPLAY_BRIGHTNESS  64
#define DISPLAY_CHIPSET     WS2811
#define DISPLAY_MATRIX_W    12
#define DISPLAY_MATRIX_H    20
#define DISPLAY_NUM_LEDS    (DISPLAY_MATRIX_W * DISPLAY_MATRIX_H)
#define DISPLAY_FPS         30

/*
  X,Y 0,0 = bottom left corner for player 1
*/

class Display
{
  public:
    void    setup();
    void    update();
    void    set_pixel(uint8_t x, uint8_t y, CRGB color);
    void    set_pixel(uint8_t x, uint8_t y, uint8_t color);
    void    clear_pixel(uint8_t x, uint8_t y);
    uint8_t get_pixel(uint8_t x, uint8_t y);
    void    clear_pixels();
    
  private:
    uint16_t coords_to_index(uint8_t x, uint8_t y);

    unsigned long _last_update;
    bool _cleared;
    
    CRGB _framebuffer[DISPLAY_NUM_LEDS];

    CRGB _palette[15] = {
      CRGB::Black,
      CRGB::Blue,
      CRGB::Gray,
      CRGB::Red,
      CRGB::Orange,
      CRGB::Green,
      CRGB::SeaGreen,
      CRGB::Violet,
      CRGB::Yellow,
      CRGB::LightBlue,
      CRGB::LightGreen,
      CRGB::Pink,
      CRGB::DarkBlue,
      CRGB::DarkRed,
      CRGB::DarkGreen
    };
};

#endif