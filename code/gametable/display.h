#ifndef display_h
#define display_h

#define FASTLED_INTERNAL  // Hides pragma messages

#include "Arduino.h"
#include <FastLED.h>

#define DISPLAY_PIN_LEDS    9
#define DISPLAY_BRIGHTNESS  64
#define DISPLAY_CHIPSET     WS2811
#define DISPLAY_MATRIX_W    12
#define DISPLAY_MATRIX_H    20
#define DISPLAY_NUM_LEDS    (DISPLAY_MATRIX_W * DISPLAY_MATRIX_H)
#define DISPLAY_FPS         20

class Display
{
  public:
    void    setup();
    void    update();
    void    set_pixel(uint8_t x, uint8_t y, CRGB color);
    void    set_pixel(uint8_t x, uint8_t y, uint8_t palette_color);
    void    clear_pixel(uint8_t x, uint8_t y);
    uint8_t get_pixel(uint8_t x, uint8_t y);
    void    clear_pixels();
    
  private:
    uint16_t coords_to_index(uint8_t x, uint8_t y);

    unsigned long _last_update;
    bool _framebuffer_updated;
    
    CRGB _framebuffer[DISPLAY_NUM_LEDS];

    CRGB _palette[12] = {
      CRGB::Black,      // 0
      CRGB::WhiteSmoke, // 1
      CRGB::Blue,       // 2
      CRGB::Red,        // 3
      CRGB::DarkOrange, // 4
      CRGB::Green,      // 5
      CRGB::Purple,     // 6
      CRGB::Sienna,     // 7
      CRGB::DodgerBlue, // 8
      CRGB::Yellow      // 9
    };
};

#endif