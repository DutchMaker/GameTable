#include "menu.h"

// Macro for retrieving menu options data from program memory.
#define MENU_DATA_OPTIONS(option,pixel,xyc) (uint8_t)(pgm_read_byte(&menu_data_options[option][pixel][xyc]))

// Store static menu options data in program memory.
const PROGMEM uint8_t menu_data_options[3][7][3] =
{
  { { 3, 16, 3 }, { 5, 16, 9 }, { 6, 16, 4 }, { 7, 16, 4 }, { 8, 16, 4 }, { 0, 0, 0 }, { 0, 0, 0 } },
  { { 5, 10, 5 }, { 6, 10, 5 }, { 7, 10, 5 }, { 6, 11, 5 }, { 0, 0, 0 },  { 0, 0, 0 }, { 0, 0, 0 } },
  { { 3, 3, 1 },  { 3, 4, 1 },  { 3, 5, 1 },  { 5, 4, 3 },  { 8, 3, 1 },  { 8, 4, 1 }, { 8, 5, 1 } }
};

void Menu::start(Display* display, Controller* controller)
{
  _display = display;
  _controller = controller;
  
  selected_game = 0;
  run_game = false;

  _button_state = 0;
  _visible_state = true;
  
  randomSeed(analogRead(0));
}

void Menu::show_menu()
{
  selected_game = 0;
  run_game = false;

  _visible_state = true;
}

void Menu::update()
{
  if (millis() - _last_update > MENU_UPDATE_SPEED)
  {
    _display->clear_pixels();

    handle_input();
    draw_options();

    _last_update = millis();
  }
}

void Menu::draw_options()
{
  for (uint8_t game = 0; game < MENU_NUM_GAMES; game++)
  {
    for (uint8_t i = 0; i < 7; i++)
    {
      uint8_t x = MENU_DATA_OPTIONS(game, i, 0);
      uint8_t y = MENU_DATA_OPTIONS(game, i, 1);
      uint8_t c = MENU_DATA_OPTIONS(game, i, 2);

      if (x == 0 && y == 0 && c == 0)
      {
        continue;
      }

      if (game == selected_game)
      {
        if (_visible_state)
        {
          _display->set_pixel(x, y, c);
        }
        else
        {
          _display->clear_pixel(x, y);
        }
      }
      else
      {
        _display->set_pixel(x, y, c);
      }
    }
  }

  switch (_button_state++)
  {
    case 0:
      _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_BLINK_UP);
      break;
    case 1:
      _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_BLINK_DOWN);
      break;
    case 2:
      // Hardware bug: Player 1 blink right will actually blink the left button.
      _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_BLINK_LEFT);
      break;
  }

  if (_button_state > 2)
  {
    _button_state = 0;
  }

  _visible_state = !_visible_state;
}

void Menu::handle_input()
{
  int8_t button = _controller->take_button_from_queue(CONTROLLER_PLAYER1);

  if (button == CONTROLLER_BIT_RIGHT) 
  {
    _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_OFF);

    // Start selected game
    run_game = true;
    randomSeed(millis());

    return;
  }

  if (button == CONTROLLER_BIT_DOWN)  
  {
    // Select previous game
    selected_game++;

    if (selected_game > MENU_NUM_GAMES - 1)
    {
      selected_game = 0;
    }

    randomSeed(millis());

    return;
  }

  if (button == CONTROLLER_BIT_UP)  
  {
    // Select next game
    selected_game--;

    if (selected_game < 0)
    {
      selected_game = MENU_NUM_GAMES - 1;
    }

    randomSeed(millis());

    return;
  }
}