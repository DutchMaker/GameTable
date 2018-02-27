#include "display.h"

//CRGB* const leds(DISPLAY_NUM_LEDS);

// Initialize the display.
void Display::setup()
{
  FastLED.addLeds<DISPLAY_CHIPSET, DISPLAY_PIN_LEDS, GRB>(_framebuffer, DISPLAY_NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(DISPLAY_BRIGHTNESS);

  clear_pixels();
}

// Update the display according to the data in the framebuffer.
void Display::update()
{
  if (millis() - _last_update < (1000 / DISPLAY_FPS))
  {
    return;
  }

  // TODO: Check if framebuffer changed since last update

  FastLED.show();

  _last_update = millis();
}

// Set the value of the pixel at specified location.
void Display::set_pixel(uint8_t x, uint8_t y, CRGB color)
{
  _framebuffer[coords_to_index(x, y)] = color;
}

void Display::set_pixel(uint8_t x, uint8_t y, uint8_t color)
{
  set_pixel(x, y, _palette[color]);
}

// Clear the value of the pixel at specified location.
void Display::clear_pixel(uint8_t x, uint8_t y)
{
  set_pixel(x, y, CRGB::Black);
}

// Get the value of the pixel at specified location.
uint8_t Display::get_pixel(uint8_t x, uint8_t y)
{
  return _framebuffer[coords_to_index(x, y)];
}

// Clears all pixel values.
void Display::clear_pixels()
{
  for (uint16_t i = 0; i < DISPLAY_NUM_LEDS; i++)
  {
    _framebuffer[i] = 0;
  }
}

// Convert coordinates to framebuffer index.
uint16_t Display::coords_to_index(uint8_t x, uint8_t y)
{
  if (x % 2 == 0)
  {
    return x * DISPLAY_MATRIX_H + y;
  }
  else
  {
    return x * DISPLAY_MATRIX_H - 1 + (DISPLAY_MATRIX_H - y);
  }  
}