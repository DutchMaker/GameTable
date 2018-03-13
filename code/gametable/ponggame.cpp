#include "ponggame.h"

// Start Pong game.
void PongGame::start(Display* display, NumericDisplay* numeric_displays, Controller* controller, Countdown* countdown, Menu* menu)
{
  _display          = display;
  _numeric_displays = numeric_displays;
  _controller       = controller;
  _countdown        = countdown;
  _menu             = menu;

  restart();
}

void PongGame::restart()
{
  _countdown->reset();

  _game_state = PONG_GAMESTATE_COUNTDOWN;
  
  // Set player 1 paddle X/Y
  _paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X] = 2;
  _paddle_location[CONTROLLER_PLAYER1][PONG_COORD_Y] = 0;

  // Set player 2 paddle X/Y
  _paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X] = 6;
  _paddle_location[CONTROLLER_PLAYER2][PONG_COORD_Y] = 19;

  _display->clear_pixels();

  _controller->reset_queues();
}

void PongGame::start_game()
{
  _score_player1 = 0;
  _score_player2 = 0;

  _bullet_count[0] = 0;
  _bullet_count[1] = 0;

  _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_ON);
  _controller->set_light_state(CONTROLLER_PLAYER2, CONTROLLER_LIGHT_STATE_ON);
    
  _numeric_displays->on();
  _numeric_displays->set_values(0);

  _controller->active_player = CONTROLLER_PLAYER1;
  _display->set_orientation(true);

  _display->clear_pixels();
}

// Game loop.
void PongGame::update()
{
  switch (_game_state)
  {
    case PONG_GAMESTATE_COUNTDOWN:
      update_countdown();
      break;
    case PONG_GAMESTATE_RUNNING:
      update_game();
      break;
    case PONG_GAMESTATE_SCORED:
      update_scored();
      break;
    case PONG_GAMESTATE_DONE:
      update_done();
      break;
  }
}

// Update logic for running state.
void PongGame::update_game()
{
  if (millis() - _movement_last_update > PONG_UPDATESPEED_MOVEMENT)
  {
    handle_input();
    _movement_last_update = millis();
  }

  if (millis() - _bullets_last_update > PONG_UPDATESPEED_BULLETS)
  {
    move_bullets();
    _bullets_last_update = millis();
  }

  if (millis() - _display_last_update > PONG_UPDATESPEED_DISPLAY)
  {
    draw_game();
    _display_last_update = millis();
  }
}

// Update logic for countdown at start of game.
void PongGame::update_countdown()
{
  _countdown->update();

  if (_countdown->finished)
  {
    _game_state = PONG_GAMESTATE_RUNNING;
    start_game();
  }
}

void PongGame::update_scored()
{

}

// Update logic for dead game state.
void PongGame::update_done()
{
  if (millis() - _done_last_update < 100)
  {
    return;
  }
  
  // Blink arrow towards winning player.
  if (_done_update_state)
  {
    // draw_arrow();
  }
  else
  {
    _display->clear_pixels();
  }

  if (millis() - _done_since > 1000)
  {
    if (_controller->take_button_from_queue(CONTROLLER_PLAYER1) == CONTROLLER_BIT_UP
      || _controller->take_button_from_queue(CONTROLLER_PLAYER2) == CONTROLLER_BIT_UP)
    {
      restart();
      start_game();

      _menu->show_menu();
      _controller->reset_queues();

      return;
    }
  }
  
  _done_update_state = !_done_update_state;

  _display->update();
  _done_last_update = millis();
}

void PongGame::handle_input()
{
  int8_t button_player1 = _controller->take_button_from_queue(CONTROLLER_PLAYER1);
  int8_t button_player2 = _controller->take_button_from_queue(CONTROLLER_PLAYER2);

  if (button_player1 == CONTROLLER_BIT_RIGHT)
  {
    // Move p1 right (from player point of view)
    if (_paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X] + PONG_PADDLE_WIDTH < DISPLAY_MATRIX_W)
    {
      _paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X]++;
    }
  }
  else if (button_player1 == CONTROLLER_BIT_LEFT)
  {
    // Move p1 left (from player point of view)
    if (_paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X] > 0)
    {
      _paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X]--;
    }
  }
  else if (button_player1 == CONTROLLER_BIT_UP)
  {
    // Shoot a bullet from p1
    new_bullet(CONTROLLER_PLAYER1);
  }

  if (button_player2 == CONTROLLER_BIT_RIGHT)
  {
    // Move p2 right (from player point of view)
    if (_paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X] > 0)
    {
      _paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X]--;
    }
  }
  else if (button_player2 == CONTROLLER_BIT_LEFT)
  {
    // Move p2 left (from player point of view)
    if (_paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X] + PONG_PADDLE_WIDTH < DISPLAY_MATRIX_W)
    {
      _paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X]++;
    }
  }
  else if (button_player2 == CONTROLLER_BIT_UP)
  {
    // Shoot a bullet from p2
    new_bullet(CONTROLLER_PLAYER2);
  }

  if (button_player1 > -1 || button_player2 > -1)
  {
    _controller->reset_queues();
  }
}

void PongGame::move_bullets()
{
  for (uint8_t player = 0; player < 2; player++)
  {
    if (_bullet_count[player] == 0)
    {
      continue;
    }

    int8_t velocity = 1 - (player * 2);
    uint8_t enemy = player == CONTROLLER_PLAYER1 ? CONTROLLER_PLAYER2 : CONTROLLER_PLAYER1;

    for (uint8_t bullet = 0; bullet < _bullet_count[player]; bullet++)
    {
      _bullets[player][bullet][PONG_COORD_Y] += velocity;

      // Collission detection
      if (_bullets[player][bullet][PONG_COORD_Y] == _paddle_location[enemy][PONG_COORD_Y])
      {
        if (_bullets[player][bullet][PONG_COORD_X] >= _paddle_location[enemy][PONG_COORD_X] && _bullets[player][bullet][PONG_COORD_X] < _paddle_location[enemy][PONG_COORD_X] + PONG_PADDLE_WIDTH)
        {
          // Hit
          // TODO: Explosion animation.
          if (player == CONTROLLER_PLAYER1)
          {
            _score_player1 += 1000;
          }
          else
          {
            _score_player2 += 1000;
          }

          _bullet_count[0] = 0;
          _bullet_count[1] = 0;

          _numeric_displays->set_value(_score_player1, 0);
          _numeric_displays->set_value(_score_player2, 1);
        }
      }

      if (_bullets[player][bullet][PONG_COORD_Y] < 0 || _bullets[player][bullet][PONG_COORD_Y] > DISPLAY_MATRIX_H - 1)
      {
        // Decrease number of bullets for player.
        if (--_bullet_count[player] > 0)
        {
          for (uint8_t i = 0; i < _bullet_count[player]; i++)
          {
            _bullets[player][i][0] = _bullets[player][i + 1][0];
            _bullets[player][i][1] = _bullets[player][i + 1][1];
          }
        }
      }
    }
  }
}

void PongGame::draw_game()
{
  _display->clear_pixels();

  // Draw player paddles
  for (uint8_t pixel = 0; pixel < PONG_PADDLE_WIDTH; pixel++)
  {
    _display->set_pixel(_paddle_location[CONTROLLER_PLAYER1][PONG_COORD_X] + pixel, _paddle_location[CONTROLLER_PLAYER1][PONG_COORD_Y], PONG_COLOR_PADDLE);
    _display->set_pixel(_paddle_location[CONTROLLER_PLAYER2][PONG_COORD_X] + pixel, _paddle_location[CONTROLLER_PLAYER2][PONG_COORD_Y], PONG_COLOR_PADDLE);
  }

  // Draw bullets
  for (uint8_t player = 0; player < 2; player++)
  {
    if (_bullet_count[player] == 0)
    {
      continue;
    }

    for (uint8_t bullet = 0; bullet < _bullet_count[player]; bullet++)
    {
      _display->set_pixel(_bullets[player][bullet][PONG_COORD_X], _bullets[player][bullet][PONG_COORD_Y], PONG_COLOR_BULLET);
    }
  }
}

void PongGame::new_bullet(uint8_t player)
{
  if (millis() - _newbullet_last_update[player] < PONG_UPDATESPEED_NEWBULLET)
  {
    return;
  }

  if (_bullet_count[player] < PONG_MAX_BULLETS)
  {
    _bullet_count[player]++;

    int8_t velocity = 1 - (player * 2);

    _bullets[player][_bullet_count[player] - 1][PONG_COORD_X] = _paddle_location[player][PONG_COORD_X] + 1;
    _bullets[player][_bullet_count[player] - 1][PONG_COORD_Y] = _paddle_location[player][PONG_COORD_Y] + velocity;
  }

  _newbullet_last_update[player] = millis();
}