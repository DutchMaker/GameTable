#include "menu.h"

uint8_t options_data[2][4][3] =
{
  { { 2, 7, 1 }, { 2, 8, 7 }, { 2, 9, 7 }, { 2, 10, 7 } },
  { { 7, 7, 5 }, { 7, 8, 5 }, { 6, 8, 5 }, { 6, 9, 5 } }
};

void Menu::start(Display* display, Controller* controller)
{
  _display = display;

  _controller = controller;
  //_controller->update_speed = MENU_UPDATE_SPEED + 100;

  selected_game = 0;
  run_game = false;

  _visible_state = true;
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
  for (byte game = 0; game < 2; game++)
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

  if (button == CONTROLLER_BIT_UP)
  {
    run_game = true;
    randomSeed(millis());

    _controller->reset_queues();

    return;
  }

  if (button == CONTROLLER_BIT_RIGHT)
  {
    selected_game++;

    if (selected_game > 1)
    {
      selected_game = 0;
    }

    randomSeed(millis());
    _controller->reset_queues();

    return;
  }

  if (button == CONTROLLER_BIT_LEFT)
  {
    selected_game--;

    if (selected_game < 0)
    {
      selected_game = 1;
    }

    randomSeed(millis());
    _controller->reset_queues();

    return;
  }
}