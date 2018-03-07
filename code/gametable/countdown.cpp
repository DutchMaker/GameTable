#include "countdown.h"

// Macro for retrieving countdown data from program memory.
#define COUNTDOWN_DATA(number,row,col) (uint8_t)(pgm_read_byte(&countdown_data[number][row][col]))

// Data for numbers in countdown, stored in program memory.
const PROGMEM uint8_t countdown_data[3][7][5] =
{
  { { 0, 1, 1, 1, 0 },{ 1, 0, 0, 0, 1 },{ 0, 0, 0, 0, 1 },{ 0, 0, 1, 1, 0 },{ 0, 0, 0, 0, 1 },{ 1, 0, 0, 0, 1 },{ 0, 1, 1, 1, 0 } },
  { { 0, 1, 1, 1, 0 },{ 1, 0, 0, 0, 1 },{ 0, 0, 0, 1, 0 },{ 0, 0, 1, 0, 0 },{ 0, 1, 0, 0, 0 },{ 1, 0, 0, 0, 0 },{ 1, 1, 1, 1, 1 } },
  { { 0, 0, 1, 0, 0 },{ 0, 1, 1, 0, 0 },{ 0, 0, 1, 0, 0 },{ 0, 0, 1, 0, 0 },{ 0, 0, 1, 0, 0 },{ 0, 0, 1, 0, 0 },{ 0, 1, 1, 1, 0 } }
};

void Countdown::setup(Display* display, NumericDisplay* numeric_displays, Controller* controller)
{
  _display = display;
  _numeric_displays = numeric_displays;
  _controller = controller;

  reset();
}

void Countdown::reset()
{
  _countdown_state = -1;
  _last_update = 0;
  _text_displayed = false;
  finished = false;

  _controller->set_light_state(_controller->active_player, CONTROLLER_LIGHT_STATE_TWIRL_SHORT);
}

// Update logic for countdown state.
void Countdown::update()
{
  if (millis() - _last_update < 1000 || finished)
  {
    return;
  }

  if (++_countdown_state > 2)
  {
    finished = true;    
    return;
  }

  if (!_text_displayed)
  {
    _numeric_displays->on();
    _numeric_displays->write(_controller->active_player, 1, 'Y');
    _numeric_displays->write(_controller->active_player, 2, 'D');
    _numeric_displays->write(_controller->active_player, 3, 'A');
    _numeric_displays->write(_controller->active_player, 4, 'E');
    _numeric_displays->write(_controller->active_player, 5, 'R');

    _text_displayed = true;
  }

  // Draw countdown digit...
  uint8_t offset_y = 5;

  _display->clear_pixels();

  for (uint8_t y = 0; y < 7; y++)
  {
    for (uint8_t x = 0; x < 5; x++)
    {
      if (COUNTDOWN_DATA(_countdown_state, y, x) != 0)
      {
        if (x + 1 + (_countdown_state * 3) < 12)
        {
          uint8_t fix_y = DISPLAY_MATRIX_H - (y + offset_y) - 1;
          _display->set_pixel(x + 1 + (_countdown_state * 3), fix_y, 1);
        }
      }
    }
  }
  
  _last_update = millis();
}