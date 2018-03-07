#include "display.h"

// Initialize the display.
void Display::setup()
{
  FastLED.addLeds<DISPLAY_CHIPSET, DISPLAY_PIN_LEDS, GRB>(_framebuffer, DISPLAY_NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(DISPLAY_BRIGHTNESS);

  _player1_orientation = true;

  clear_pixels();
}

// Update the display according to the data in the framebuffer.
void Display::update()
{
  if (millis() - _last_update < (1000 / DISPLAY_FPS))
  {
    return;
  }

  if (_framebuffer_updated)
  {
    if (!_player1_orientation)
    {
      flip_framebuffer();
    }

    FastLED.show();
    _framebuffer_updated = false;
  }

  _last_update = millis();
}

// Set the color value of the pixel at specified location.
void Display::set_pixel(uint8_t x, uint8_t y, CRGB color)
{
  _framebuffer[coords_to_index(x, y)] = color;
  _framebuffer_updated = true;
}

// Set the color value of the pixel at specified location.
void Display::set_pixel(uint8_t x, uint8_t y, uint8_t palette_color)
{
  set_pixel(x, y, _palette[palette_color]);
}

// Clear the value of the pixel at specified location.
void Display::clear_pixel(uint8_t x, uint8_t y)
{
  set_pixel(x, y, CRGB::Black);
}

// Get the value of the pixel at specified location.
CRGB Display::get_pixel(uint8_t x, uint8_t y)
{
  return _framebuffer[coords_to_index(x, y)];
}

// Clears all pixel values.
void Display::clear_pixels()
{
  for (uint16_t i = 0; i < DISPLAY_NUM_LEDS; i++)
  {
    _framebuffer[i] = CRGB::Black;
  }

  _framebuffer_updated = true;
}

// Set screen orientation (player 1 is default at boot).
void Display::set_orientation(bool player1)
{
  _player1_orientation = player1;
  _framebuffer_updated = true;
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

// When orientation is set to Player 2 we (vertically) flip the framebuffer.
void Display::flip_framebuffer()
{
  for (uint8_t y = 0; y < DISPLAY_MATRIX_H_HALF; y++)
  {
    for (uint8_t x = 0; x < DISPLAY_MATRIX_W; x++)
    {
      uint8_t nx = DISPLAY_MATRIX_W - 1 - x;
      uint8_t ny = DISPLAY_MATRIX_H - 1 - y;

      CRGB move_value = _framebuffer[coords_to_index(x, y)];
      CRGB old_value = _framebuffer[coords_to_index(nx, ny)];

      _framebuffer[coords_to_index(nx, ny)] = move_value;
      _framebuffer[coords_to_index(x, y)] = old_value;
    }
  }
}