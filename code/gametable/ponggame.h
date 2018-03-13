#ifndef ponggame_h
#define ponggame_h

#include "Arduino.h"
#include "display.h"
#include "numericdisplay.h"
#include "controller.h"
#include "menu.h"
#include "countdown.h"

#define PONG_COLOR_PADDLE 1
#define PONG_COLOR_BULLET 3
#define PONG_PADDLE_WIDTH 3
#define PONG_MAX_BULLETS  3

#define PONG_COORD_X 0
#define PONG_COORD_Y 1

#define PONG_GAMESTATE_COUNTDOWN 1
#define PONG_GAMESTATE_RUNNING   2
#define PONG_GAMESTATE_SCORED    3
#define PONG_GAMESTATE_DONE      4

#define PONG_UPDATESPEED_BULLETS    100
#define PONG_UPDATESPEED_MOVEMENT   100
#define PONG_UPDATESPEED_DISPLAY    50
#define PONG_UPDATESPEED_NEWBULLET  500

class PongGame
{
  public:
    void start(Display* display, NumericDisplay* numeric_displays, Controller* controller, Countdown* countdown, Menu* menu);
    void update();
    
  private:
    void restart();
    void start_game();
    void update_game();
    void update_countdown();
    void update_scored();
    void update_done();
    void handle_input();
    void move_bullets();
    void draw_game();
    void new_bullet(uint8_t player);
    
    Display*        _display;
    NumericDisplay* _numeric_displays;
    Controller*     _controller;
    Countdown*      _countdown;
    Menu*           _menu;

    uint8_t _game_state;    
    uint8_t _paddle_location[2][2]; // [player][x/y]
    int8_t  _bullets[2][3][2];      // [player][bullet][x/y]
    uint8_t _bullet_count[2];       // [player]

    long          _score_player1;
    long          _score_player2;
    unsigned long _score_last_update;

    unsigned long _movement_last_update;
    unsigned long _bullets_last_update;
    unsigned long _display_last_update;
    unsigned long _newbullet_last_update[2];  // [player]

    bool          _done_update_state;
    unsigned long _done_last_update;
    unsigned long _done_since;
};

#endif
