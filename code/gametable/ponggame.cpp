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
  _paddle_location[CONTROLLER_PLAYER1][0] = 2;
  _paddle_location[CONTROLLER_PLAYER1][1] = 0;

  // Set player 2 paddle X/Y
  _paddle_location[CONTROLLER_PLAYER2][0] = 6;
  _paddle_location[CONTROLLER_PLAYER2][1] = 19;
  
  _display->clear_pixels();

  _controller->reset_queues();
}

void PongGame::start_game()
{
  _score[0] = 0;
  _score[1] = 0;

  _controller->set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_ON);
  _controller->set_light_state(CONTROLLER_PLAYER2, CONTROLLER_LIGHT_STATE_ON);
    
  _numeric_displays->on();
  _numeric_displays->set_values(0);

  _display->clear_pixels();

  //draw_snake();
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

  if (millis() - _ball_last_update > PONG_UPDATESPEED_BALL)
  {
    move_ball();    
    _ball_last_update = millis();
  }

  if (millis() - _display_last_update > PONG_UPDATESPEED_DISPLAY)
  {
    draw_game_screen();
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
    if (_controller->take_button_from_queue(CONTROLLER_PLAYER1) == CONTROLLER_BIT_UP)
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
  int8_t button = _controller->take_button_from_queue(CONTROLLER_PLAYER1);

  if (button == CONTROLLER_BIT_RIGHT) // && _snake_direction != SNAKE_DIR_RIGHT && _snake_direction != SNAKE_DIR_LEFT)
  {
    //_snake_direction = SNAKE_DIR_RIGHT;
    return;
  }

  if (button == CONTROLLER_BIT_LEFT) // && _snake_direction != SNAKE_DIR_LEFT && _snake_direction != SNAKE_DIR_RIGHT)
  {
    
    return;
  }
}

void PongGame::move_ball()
{

}

void PongGame::draw_game_screen()
{

}