#include "tetrisgame.h"

// Macro for retrieving the tetromino data.
#define TETRIS_DATA_TETROMINO(type,rotation,pixel,xy) (uint8_t)(pgm_read_byte(&tetris_data_tetromino[type][rotation][pixel][xy]))

// Definitions of tetromino, stored in program memory.
const PROGMEM uint8_t tetris_data_tetromino[7][4][4][2] = 
{
  // I-Piece
  {
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } },
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } }
  },
  
  // J-Piece
  {
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 0 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 0, 0 } }
  },

  // L-Piece
  {
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 0, 2 } },
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 0 } },
    { { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 0 } }
  },

  // O-Piece
  {
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
  },

  // S-Piece
  {
    { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
    { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }
  },

  // T-Piece
  {
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
    { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
    { { 1, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } }
  },

  // Z-Piece
  {
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
    { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
    { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } }
  }
};

void TetrisGame::start(Display* display, NumericDisplay* numeric_displays, Controller* controller, Countdown* countdown, Menu* menu)
{
  _display = display;
  _numeric_displays = numeric_displays;
  _menu = menu;
  _countdown = countdown;
  _controller = controller;
  
  restart();
}

void TetrisGame::start_game()
{
  _last_pieces[0] = -1;
  _last_pieces[1] = -1;

  _last_displayed_score = -1;
  _score = 0;

  _numeric_displays->on();
  _numeric_displays->set_values(0);

  _controller->set_light_state(_controller->active_player, CONTROLLER_LIGHT_STATE_ON);
  _countdown->reset();

  create_new_piece();
}

void TetrisGame::restart()
{
  clear_field();
  
  start_game();

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
  if (millis() - _game_last_move_update > TETRIS_UPDATESPEED_MOVEMENT)
  {
    handle_input();
    
    draw_field();
    draw_piece(false);

    _game_last_move_update = millis();
  }

  if (_already_moved_down)
  {
    // Skip move down (falling) if player already manually moved down.
    _game_last_fall_update = millis();
  }

  if (millis() - _game_last_fall_update > TETRIS_UPDATESPEED_FALLING)
  {
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

    draw_score();
    _game_last_fall_update = millis();
  }
}

void TetrisGame::update_countdown()
{
  _countdown->update();

  if (_countdown->finished)
  {
    _game_state = TETRIS_GAMESTATE_RUNNING;
    start_game();
  }
}

void TetrisGame::update_dead()
{
  if (millis() - _dead_last_update < TETRIS_UPDATESPEED_DEAD_BLINK)
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
    if (_controller->take_button_from_queue(_controller->active_player) == CONTROLLER_BIT_UP)
    {
      restart();
      start_game();

      _menu->show_menu();
      _controller->reset_queues();

      return;
    }
  }

  _dead_update_state = !_dead_update_state;
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
    uint8_t removed_rows = 0;

    while (row > 0)
    {
      row--;

      if (is_completed_line(row))
      {
        // Count score
        _score += (++removed_rows) * TETRIS_SCORE_PER_LINE;

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

    create_new_piece();
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
    uint8_t x = TETRIS_DATA_TETROMINO(_current_piece, _current_rotation, i, 0);
    uint8_t y = TETRIS_DATA_TETROMINO(_current_piece, _current_rotation, i, 1);

    int8_t index_x = x + _current_location[0];
    int8_t index_y = y + _current_location[1];

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
  // Only placed pieces (no longer controlled by player) are stored in the _field buffer.

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t x = TETRIS_DATA_TETROMINO(_current_piece, _current_rotation, i, 0);
    uint8_t y = TETRIS_DATA_TETROMINO(_current_piece, _current_rotation, i, 1);

    uint8_t index_x = x + _current_location[0];
    uint8_t index_y = y + _current_location[1];

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

// Create a new player controlled piece in the top of the screen.
void TetrisGame::create_new_piece()
{
    _current_location[0] = 5;
    _current_location[1] = -1;
    _current_rotation = 0;

    // Select a random tetris piece and prevent the same piece from being repeates more than twice.
    do
    {
      _current_piece = (uint8_t)random(0, 7);
    }
    while (_last_pieces[0] == _current_piece && _last_pieces[1] == _current_piece);

    _last_pieces[1] = _last_pieces[0];
    _last_pieces[0] = _current_piece;
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

void TetrisGame::draw_score()
{
  if (_score != _last_displayed_score)
  {
    _numeric_displays->set_value(_score, _controller->active_player);
    _last_displayed_score = _score;
  }
}

void TetrisGame::handle_input()
{
  int8_t button = _controller->take_button_from_queue(_controller->active_player);
  _already_moved_down = false;

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

    return;
  }

  if (button == CONTROLLER_BIT_RIGHT)
  {
    uint8_t previous_x = _current_location[0];
    _current_location[0]++;

    if (check_collision())
    {
      _current_location[0] = previous_x;
    }

    _controller->reset_queues();
    
    return;
  }

  if (button == CONTROLLER_BIT_DOWN)
  {
    _already_moved_down = true;
    move_down();

    _controller->reset_queues();

    return;
  }

  if (button == CONTROLLER_BIT_LEFT)
  {
    uint8_t previous_x = _current_location[0];
    _current_location[0]--;

    if (check_collision())
    {
      _current_location[0] = previous_x;
    }

    _controller->reset_queues();
    
    return;
  }
}

// Moves the piece down. Returns true if piece was placed (reached bottom).
bool TetrisGame::move_down()
{
  uint8_t previous_y = _current_location[1];
  _current_location[1]++;

  if (check_collision())
  {
    _current_location[1] = previous_y;

    // Draw the piece to the field buffer
    draw_piece(true);
    _display->clear_pixels();

    // Remove lines that are complete.
    if (lines_completed())
    {
      _game_state = TETRIS_GAMESTATE_REMOVELINES;
      _removeline_state = 0;

      return true;
    }

    // If we have collision on move_down and the piece is still at the top;
    // we reached the end of the game.
    if (_current_location[1] == 0 || _current_location[1] == 1)
    {
      _game_state = TETRIS_GAMESTATE_DEAD;
      _dead_since = millis();

      _controller->set_light_state(_controller->active_player, CONTROLLER_LIGHT_STATE_BLINK_UP);
      _controller->reset_queues();

      return true;
    }

    create_new_piece();

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
  for (uint8_t col = 0; col < 12; col++)
  {
    set_field_pixel(col, row, color);
  }
}