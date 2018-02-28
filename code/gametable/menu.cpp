#include "menu.h"

uint8_t options_data[3][7][3] =
{
  { { 4, 15, 3 }, { 6, 15, 9 }, { 7, 15, 4 }, { 9, 15, 4 }, { 9, 15, 4 } },
  { { 5, 9, 5 }, { 6, 9, 5 }, { 7, 9, 5 }, { 6, 10, 5 } },
  { { 3, 2, 1 }, { 3, 3, 1 }, { 3, 4, 1 }, { 5, 3, 3 }, { 8, 2, 1 }, { 8, 3, 1 }, { 8, 3, 1 } }
};

void Menu::start(Display* display, Controller* controller)
{
  _display = display;
  _controller = controller;
  
  selected_game = 0;
  run_game = false;

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
  for (byte game = 0; game < MENU_NUM_GAMES; game++)
  {
    for (byte i = 0; i < 4; i++)
    {
      byte x = options_data[game][i][0];
      byte y = options_data[game][i][1];
      byte c = options_data[game][i][2];

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

  _visible_state = !_visible_state;
}

void Menu::handle_input()
{
  byte button = _controller->take_button_from_queue(CONTROLLER_PLAYER1);

  if (button == 0)
  {
    return;
  }

  if (button == CONTROLLER_BIT_RIGHT) 
  {
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