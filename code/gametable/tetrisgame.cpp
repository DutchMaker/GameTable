#include "tetrisgame.h"

Point tetromino[7][4][4] = 
{
  // I-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(3, 1) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(1, 3) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(3, 1) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(1, 3) }
  },
  
  // J-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(2, 0) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(2, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(0, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(0, 0) }
  },

  // L-Piece
  {
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(2, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(0, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(0, 0) },
    { get_point(1, 0), get_point(1, 1), get_point(1, 2), get_point(2, 0) }
  },

  // O-Piece
  {
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 0), get_point(1, 1) }
  },

  // S-Piece
  {
    { get_point(1, 0), get_point(2, 0), get_point(0, 1), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) },
    { get_point(1, 0), get_point(2, 0), get_point(0, 1), get_point(1, 1) },
    { get_point(0, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) }
  },

  // T-Piece
  {
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(1, 2) },
    { get_point(0, 1), get_point(1, 1), get_point(2, 1), get_point(1, 2) },
    { get_point(1, 0), get_point(1, 1), get_point(2, 1), get_point(1, 2) }
  },

  // Z-Piece
  {
    { get_point(0, 0), get_point(1, 0), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(0, 2) },
    { get_point(0, 0), get_point(1, 0), get_point(1, 1), get_point(2, 1) },
    { get_point(1, 0), get_point(0, 1), get_point(1, 1), get_point(0, 2) }
  }
};

void TetrisGame::start(Display* display, Controller* controller, Menu* menu)
{
  _display = display;
  _menu = menu;
  
  _controller = controller;
  //_controller->update_speed = TETRIS_GAME_SPEED;
  
  restart();
}

void TetrisGame::start_game()
{
  _current_location = get_point(5, -1);
  _current_rotation = 0;
  _current_piece = (byte)random(0, 7);
}

void TetrisGame::restart()
{
  _current_location = get_point(5, -1);
  _current_rotation = 0;
  _current_piece = (byte)random(0, 7);

  clear_field();

  _countdown_state = -1;
  _game_state = TETRIS_GAMESTATE_COUNTDOWN;
}

void TetrisGame::update()
{
  switch (_game_state)
  {
    case TETRIS_GAMESTATE_COUNTDOWN:
      update_countdown();
      break;
    case TETRIS_GAMESTATE_RUNNING:
      update_game();
      break;
    case TETRIS_GAMESTATE_DEAD:
      update_dead();
      break;
    case TETRIS_GAMESTATE_REMOVELINES:
      update_removelines();
      break;
  }
}

void TetrisGame::update_game()
{
  if (millis() - _game_last_move_update > TETRIS_MOVE_SPEED)
  {
    handle_input();
    
    draw_field();
    draw_piece(false);

    _game_last_move_update = millis();
  }

  if (millis() - _game_last_update > TETRIS_GAME_SPEED)
  {
    _display->clear_pixels();

    if (!move_down())
    {
      handle_input();

      draw_field();
      draw_piece(false);
    }
    else
    {
      draw_field();
    }
    
    _game_last_update = millis();
  }
}

void TetrisGame::update_countdown()
{
  if (millis() - _countdown_last_update < 1000)
  {
    return;
  }

  if (++_countdown_state > 2)
  {
    _game_state = TETRIS_GAMESTATE_RUNNING;
    start_game();

    return;
  }

  // Draw countdown digit...
  uint8_t offset_y = 5;

  _display->clear_pixels();

  for (uint8_t y = 0; y < 7; y++)
  {
    for (uint8_t x = 0; x < 5; x++)
    {
      if (countdown_data[_countdown_state][y][x] != 0)
      {
        if (x + 1 + (_countdown_state * 2) < 12)
        {
          uint8_t fix_y = DISPLAY_MATRIX_H - (y + offset_y) - 1;
          _display->set_pixel(x + 1 + (_countdown_state * 2), fix_y, 7);
        }
      }
    }
  }

  _display->update();
  _countdown_last_update = millis();
}

void TetrisGame::update_dead()
{
  if (millis() - _dead_last_update < 100)
  {
    return;
  }

  // Blink the playing field.
  if (_dead_update_state)
  {
    draw_field();
  }
  else
  {
    _display->clear_pixels();
  }

  if (millis() - _dead_since > 1000)
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

  _dead_update_state = !_dead_update_state;

  _display->update();
  _dead_last_update = millis();
}

void TetrisGame::update_removelines()
{
  if (millis() - _removeline_last_update < 100)
  {
    return;
  }

  _removeline_state++;

  if (_removeline_state < 7)
  {
    // Update color of removable lines.
    for (uint8_t row = 0; row < 20; row++)
    {
      if (is_completed_line(row))
      {
        change_line_color(row, _removeline_state);
      }
    }

    draw_field();
  }
  else
  {
    // Remove lines
    uint8_t row = 20;

    while (row > 0)
    {
      row--;

      if (is_completed_line(row))
      {
        if (row == 0)
        {
          // Remove line
          for (uint8_t col = 0; col < 12; col++)
          {
            set_field_pixel(col, row, 0);
          }

          break;
        }

        // Remove complete lines and move lines above it down.
        int8_t move_row = row - 1;

        while (move_row >= 0)
        {
          for (uint8_t col = 0; col < 12; col++)
          {
            _field[move_row + 1][col] = _field[move_row][col];
          }

          move_row--;
        }

        row++;
      }
    }

    _game_state = TETRIS_GAMESTATE_RUNNING;
    _controller->reset_queues();

    // Create a new piece
    _current_location.x = 5;
    _current_location.y = -1;

    _current_piece = (byte)random(0, 7);
    _current_rotation = 0;
  }

  _removeline_last_update = millis();
}

void TetrisGame::clear_field()
{
  for (uint8_t y = 0; y < 20; y++)
  {
    for (uint8_t x = 0; x < 12; x++)
    {
      _field[y][x] = 0;
    }
  }
}

void TetrisGame::set_field_pixel(uint8_t x, uint8_t y, uint8_t color)
{
  _field[y][x] = color;
}

// Checks wether the piece overlaps with anything in the _field buffer.
// If any part of the piece is located outside of the screen, it also returns a collision.
bool TetrisGame::check_collision()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t x = tetromino[_current_piece][_current_rotation][i].x;
    uint8_t y = tetromino[_current_piece][_current_rotation][i].y;

    int8_t index_x = x + _current_location.x;
    int8_t index_y = y + _current_location.y;

    if (index_x < 0 || index_x > 11)
    {
      return true;
    }

    if (index_y > 19 && index_y != 255)
    {
      return true;
    }

    if (index_y < 0 || index_x < 0)
    {
      continue;
    }

    if (_field[index_y][index_x] != 0)
    {
      return true;
    }
  }

  return false;
}

void TetrisGame::draw_piece(bool field_buffer)
{
  // Piece is stored directly in the framebuffer.
  // Only placed pieces are stored in the _field buffer.

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t x = tetromino[_current_piece][_current_rotation][i].x;
    uint8_t y = tetromino[_current_piece][_current_rotation][i].y;

    uint8_t index_x = x + _current_location.x;
    uint8_t index_y = y + _current_location.y;

    if (index_x > 11)
    {
      continue;
    }

    if (index_y > 19)
    {
      continue;
    }

    if (field_buffer)
    {
      set_field_pixel(index_x, index_y, _current_piece + 1);
    }
    else
    {
      _display->set_pixel(index_x, DISPLAY_MATRIX_H - index_y - 1, _current_piece + 1);
    }
  }
}

void TetrisGame::draw_field()
{
  for (uint8_t row = 0; row < 20; row++)
  {
    for (uint8_t col = 0; col < 12; col++)
    {
      uint8_t fix_row = DISPLAY_MATRIX_H - row - 1;
      _display->set_pixel(col, fix_row, _field[row][col]);
    }
  }
}

void TetrisGame::handle_input()
{
  int8_t button = _controller->take_button_from_queue(CONTROLLER_PLAYER1);

  if (button == -1)
  {
    return;
  }
  
  if (button == CONTROLLER_BIT_UP)
  {
    uint8_t previous_rotation = _current_rotation;
    _current_rotation++;

    if (_current_rotation > 3)
    {
      _current_rotation = 0;
    }

    if (check_collision())
    {
      _current_rotation = previous_rotation;
    }

    //_controller->reset_queues();
    
    return;
  }

  if (button == CONTROLLER_BIT_RIGHT)
  {
    uint8_t previous_x = _current_location.x;
    _current_location.x++;

    if (check_collision())
    {
      _current_location.x = previous_x;
    }

    //_controller->reset_queues();
    
    return;
  }

  if (button == CONTROLLER_BIT_DOWN)
  {
    _game_last_update -= TETRIS_GAME_SPEED;
    //_controller->reset_queues();

    return;
  }

  if (button == CONTROLLER_BIT_LEFT)
  {
    uint8_t previous_x = _current_location.x;
    _current_location.x--;

    if (check_collision())
    {
      _current_location.x = previous_x;
    }

    //_controller->reset_queues();
    
    return;
  }
}

// Moves the piece down. Returns true if piece was placed (reached bottom).
bool TetrisGame::move_down()
{
  uint8_t previous_y = _current_location.y;
  _current_location.y++;

  if (check_collision())
  {
    _current_location.y = previous_y;

    // Draw the piece to the field buffer
    draw_piece(true);
    _display->clear_pixels();

    // Remove lines
    if (lines_completed())
    {
      _game_state = TETRIS_GAMESTATE_REMOVELINES;
      _removeline_state = 0;
      return true;
    }

    // If we have collision on move_down and the piece is still at the top,
    // we reached the end of the game.
    if (_current_location.y == 0 || _current_location.y == 1)
    {
      _game_state = TETRIS_GAMESTATE_DEAD;
      _dead_since = millis();
      return true;
    }

    // Create a new piece
    _current_location.x = 5;
    _current_location.y = -1;

    _current_piece = (uint8_t)random(0, 7);
    _current_rotation = 0;

    return true;
  }

  return false;
}

bool TetrisGame::lines_completed()
{
  for (uint8_t row = 0; row < 20; row++)
  {
    if (is_completed_line(row))
    {
      return true;
    }
  }

  return false;
}

bool TetrisGame::is_completed_line(uint8_t row)
{
  bool row_complete = true;

  for (uint8_t col = 0; col < 12; col++)
  {
    if (_field[row][col] == 0)
    {
      row_complete = false;
      break;
    }
  }

  return row_complete;
}

void TetrisGame::change_line_color(uint8_t row, uint8_t color)
{
  for (byte col = 0; col < 12; col++)
  {
    set_field_pixel(col, row, color);
  }
}