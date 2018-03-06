#ifndef tetrisgame_h
#define tetrisgame_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "countdown.h"
#include "menu.h"

#define TETRIS_GAMESTATE_COUNTDOWN    1
#define TETRIS_GAMESTATE_RUNNING      2
#define TETRIS_GAMESTATE_DEAD         3
#define TETRIS_GAMESTATE_REMOVELINES  4

#define TETRIS_SCORE_PER_LINE 100

#define TETRIS_UPDATESPEED_MOVEMENT   100
#define TETRIS_UPDATESPEED_FALLING    400
#define TETRIS_UPDATESPEED_DEAD_BLINK 200

class TetrisGame
{
  public:
    void start(Display* display, NumericDisplay* numeric_displays, Controller* controller, Countdown* countdown, Menu* menu);
    void update();
    
  private:
    void restart();
    void start_game();

    void update_game();
    void update_countdown();
    void update_dead();
    void update_removelines();

    void draw_piece(bool field_buffer);
    void draw_field();
    void draw_score();
    void create_new_piece();

    void handle_input();

    void clear_field();
    void set_field_pixel(uint8_t x, uint8_t y, uint8_t color);

    bool check_collision();
    bool move_down();
    bool lines_completed();
    bool is_completed_line(uint8_t row);
    void change_line_color(uint8_t row, uint8_t color);

    Display*        _display;
    NumericDisplay* _numeric_displays;
    Controller*     _controller;
    Countdown*      _countdown;
    Menu*           _menu;

    uint8_t _field[DISPLAY_MATRIX_H][DISPLAY_MATRIX_W]; // Playing fields
    
    uint8_t       _game_state;
    unsigned long _game_last_fall_update;
    unsigned long _game_last_move_update;
    bool          _already_moved_down;
    
    long _score;
    long _last_displayed_score;

    bool          _dead_update_state;
    unsigned long _dead_last_update;
    unsigned long _dead_since;

    uint8_t _current_piece;
    int8_t  _last_pieces[2] = { -1, -1 };
    uint8_t _current_rotation;
    uint8_t _current_location[2];

    uint8_t       _removeline_state;
    unsigned long _removeline_last_update;
};

#endif