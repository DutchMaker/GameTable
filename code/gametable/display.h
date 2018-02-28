#ifndef display_h
#define display_h

#define FASTLED_INTERNAL  // Hides pragma messages

#include "Arduino.h"
#include <FastLED.h>

#define DISPLAY_PIN_LEDS    9
#define DISPLAY_BRIGHTNESS  96 // TODO: Test different brightness
#define DISPLAY_CHIPSET     WS2811
#define DISPLAY_MATRIX_W    12
#define DISPLAY_MATRIX_H    20
#define DISPLAY_NUM_LEDS    (DISPLAY_MATRIX_W * DISPLAY_MATRIX_H)
#define DISPLAY_FPS         30

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
      CRGB::WhiteSmoke, // 1  (TODO: maybe replace with gray or silver)
      CRGB::Blue,       // 2
      CRGB::Red,        // 3
      CRGB::Orange,     // 4
      CRGB::Green,      // 5  (maybe LimeGreen?)
      CRGB::Sienna,     // 6
      CRGB::Purple,     // 7

      CRGB::Cyan,       // 8
      CRGB::Gold,       // 9  (Maybe Yellow?)
      CRGB::DeepPink,   // 10

      CRGB::DodgerBlue, // 11 (maybe replacement for Blue)
    };
};

#endif