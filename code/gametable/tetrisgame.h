#ifndef tetrisgame_h
#define tetrisgame_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "point.h"
#include "countdown_data.h"
#include "menu.h"

#define TETRIS_GAMESTATE_COUNTDOWN 1
#define TETRIS_GAMESTATE_RUNNING 2
#define TETRIS_GAMESTATE_DEAD 3
#define TETRIS_GAMESTATE_REMOVELINES 4

#define TETRIS_SCORE_PER_LINE 100

#define TETRIS_GAME_SPEED 400  // millis per update, lower = faster.
#define TETRIS_MOVE_SPEED 100

class TetrisGame
{
  public:
    void start(Display* display, Controller* controller, Menu* menu);
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

    Display*    _display;
    Controller* _controller;
    Menu*       _menu;

    uint8_t _field[20][12];       // Playing field pixels (TODO: Remove the need for this)
    
    uint8_t       _game_state;
    unsigned long _game_last_update;
    unsigned long _game_last_move_update;
    
    unsigned long _score;
    unsigned long _last_displayed_score;
    
    uint8_t       _countdown_state;       // TODO: MOVE COUNTDOWN TO OWN CLASS
    unsigned long _countdown_last_update; // Last time countdown state update was performed.

    bool          _dead_update_state;
    unsigned long _dead_last_update;
    unsigned long _dead_since;

    uint8_t _current_piece;
    int8_t  _last_pieces[2] = { -1, -1 };
    uint8_t _current_rotation;
    Point   _current_location;

    uint8_t       _removeline_state;
    unsigned long _removeline_last_update;
};

#endif