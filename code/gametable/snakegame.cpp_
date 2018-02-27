#include "snakegame.h"

// Start snake game.
void SnakeGame::start(Display* display, Controller* controller, Menu* menu)
{
  _display = display;
  _controller = controller;
  _menu = menu;

  restart();
}

void SnakeGame::restart()
{
  _countdown_state = -1;
  _game_state = SNAKE_GAMESTATE_COUNTDOWN;
  _snake_direction = SNAKE_DIR_RIGHT;

  _display->clear_pixels();

  _controller->reset_queue();
}

void SnakeGame::start_game()
{
  _score = 0;
  
  // Reset snake
  for (int i = 0; i < 200; i++)
  {
    _snake[i][0] = 0;
    _snake[i][1] = 0;
  }

  _snake_length = 3;

  // Setup initial snake;
  // end of the snake is at top of the array
  // head is at the end of the array
  _snake[0][0] = 1; // x
  _snake[0][1] = 4; // y
  _snake[1][0] = 2;
  _snake[1][1] = 4;
  _snake[2][0] = 3;
  _snake[2][1] = 4;

  _display->clear_pixels();

  draw_snake();
  spawn_food();
}

// Game loop.
void SnakeGame::update()
{
  switch (_game_state)
  {
    case SNAKE_GAMESTATE_COUNTDOWN:
      update_countdown();
      break;
    case SNAKE_GAMESTATE_RUNNING:
      update_game();
      break;
     case SNAKE_GAMESTATE_DEAD:
      update_dead();
      break;
  }
}

// Update logic for running state.
void SnakeGame::update_game()
{
  if (millis() - _game_last_update > SNAKE_GAME_SPEED)
  {
    update_direction();
    move_snake();
    draw_snake();

    if (_spawn_new_food)
    {
      spawn_food();
    }
    
    _game_last_update = millis();
  }

  if (millis() - _score_last_update >= 1000)
  {
    _score += 10;
    _score_last_update = millis();
  }
}

// Update logic for countdown state.
void SnakeGame::update_countdown()
{
  if (millis() - _countdown_last_update < 1000)
  {
    return;
  }

  if (++_countdown_state > 2)
  {
    _game_state = SNAKE_GAMESTATE_RUNNING;
    start_game();
    
    return;
  }

  // Draw countdown digit...
  byte offset_y = 5;

  _display->clear_pixels();

  for (byte y = 0; y < 7; y++)
  {
    for (byte x = 0; x < 5; x++)
    {
      if (countdown_data[_countdown_state][y][x] != 0)
      {
        if (x + 1 + (_countdown_state * 2) < 10)
        {
          _display->set_pixel(x + 1 + (_countdown_state * 2), y + offset_y, 7);
        }
      }
    }
  }

  _display->update();
  _countdown_last_update = millis();
}

// Update logic for dead game state.
void SnakeGame::update_dead()
{
  if (millis() - _dead_last_update < 100)
  {
    return;
  }
  
  // Blink the snake.
  if (_dead_update_state)
  {
    draw_snake();
  }
  else
  {
    _display->clear_pixels();
  }

  if (millis() - _dead_since > 1000)
  {
    if (_controller->get_button_from_queue() == CONTROLLER_BIT_UP)
    {
      restart();
      start_game();

      _menu->show_menu();
      _controller->reset_queue();

      return;
    }
  }
  
  _dead_update_state = !_dead_update_state;

  _display->update();
  _dead_last_update = millis();
}

// Spawn new food at a random location.
void SnakeGame::spawn_food()
{
  _food_x = (byte)random(0, 9);
  _food_y = (byte)random(0, 19);

  if (_display->get_pixel(_food_x, _food_y) != 0)
  {
    // If another pixel is already present at this location, cancel and spawn again.
    spawn_food();
  }

  _display->set_pixel(_food_x, _food_y, SNAKE_COLOR_FOOD);

  _spawn_new_food = false;  // Remember that we don't need to spawn new food for next update.
}

// Move the snake one step.
void SnakeGame::move_snake()
{
  byte prev_x = _snake[_snake_length - 1][0];
  byte prev_y = _snake[_snake_length - 1][1];
  
  for (int i = _snake_length - 1; i >= 0; i--)
  {
    // Move the head.
    if (i == _snake_length - 1)
    {
      byte new_x = _snake[i][0];
      byte new_y = _snake[i][1];
      
      switch (_snake_direction)
      {
        case SNAKE_DIR_UP:
          if (_snake[i][1] > 0)
          {
            new_y--;
          }
          else
          {
            new_y = 19;
          }
          break;
        case SNAKE_DIR_RIGHT:
          if (_snake[i][0] < 9)
          {
            new_x++;
          }
          else
          {
            new_x = 0;
          }
          break;
        case SNAKE_DIR_DOWN:
          if (_snake[i][1] < 19)
          {
            new_y++;
          }
          else
          {
            new_y = 0;
          }
          break;
        case SNAKE_DIR_LEFT:
          if (_snake[i][0] > 0)
          {
            new_x--;
          }
          else
          {
            new_x = 9;
          }
          break;
      }

      // Check if head touches a body part.
      for (byte j = 0; j < _snake_length - 1; j++)
      {
        if (_snake[j][0] == new_x && _snake[j][1] == new_y)
        {
          _game_state = SNAKE_GAMESTATE_DEAD;
          _dead_update_state = false;
          _dead_since = millis();
          return; 
        }
      }

      // Check if the head touches food.
      if (new_x == _food_x && new_y == _food_y)
      {
        // Add body part to snake.
        _snake[i + 1][0] = new_x;
        _snake[i + 1][1] = new_y;

        _snake_length++;
        _spawn_new_food = true;

        _score += 1000;

        break;
      }

      _snake[i][0] = new_x;
      _snake[i][1] = new_y;
    }
    else
    {
      // Move body parts
      byte cur_x = _snake[i][0];
      byte cur_y = _snake[i][1];
  
      _snake[i][0] = prev_x;
      _snake[i][1] = prev_y;

      prev_x = cur_x;
      prev_y = cur_y;
    }

    // Clear pixel at previous body part location.
    _display->clear_pixel(prev_x, prev_y);
  }
}

// Draw the snake in the display framebuffer.
void SnakeGame::draw_snake()
{
  for (int i = 0; i < _snake_length; i++)
  {
    if (i == _snake_length - 1)
    {
      _display->set_pixel(_snake[i][0], _snake[i][1], SNAKE_COLOR_HEAD);
    }
    else
    {
      _display->set_pixel(_snake[i][0], _snake[i][1], SNAKE_COLOR_BODY);
    }
  }
}

// Update the direction in which the snake is traveling.
void SnakeGame::update_direction()
{
  byte button = _controller->get_button_from_queue();

  if (button == 0)
  {
    return;
  }
  
  if (button == CONTROLLER_BIT_UP && _snake_direction != SNAKE_DIR_UP && _snake_direction != SNAKE_DIR_DOWN)
  {
    _snake_direction = SNAKE_DIR_UP;
    return;
  }

  if (button == CONTROLLER_BIT_RIGHT && _snake_direction != SNAKE_DIR_RIGHT && _snake_direction != SNAKE_DIR_LEFT)
  {
    _snake_direction = SNAKE_DIR_RIGHT;
    return;
  }

  if (button == CONTROLLER_BIT_DOWN && _snake_direction != SNAKE_DIR_DOWN && _snake_direction != SNAKE_DIR_UP)
  {
    _snake_direction = SNAKE_DIR_DOWN;
    return;
  }

  if (button == CONTROLLER_BIT_LEFT && _snake_direction != SNAKE_DIR_LEFT && _snake_direction != SNAKE_DIR_RIGHT)
  {
    _snake_direction = SNAKE_DIR_LEFT;
    return;
  }
}

