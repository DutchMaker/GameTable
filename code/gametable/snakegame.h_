#ifndef snakegame_h
#define snakegame_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "menu.h"
#include "countdown_data.h"

#define SNAKE_DIR_UP 0
#define SNAKE_DIR_RIGHT 1
#define SNAKE_DIR_DOWN 2
#define SNAKE_DIR_LEFT 3

#define SNAKE_COLOR_HEAD 1
#define SNAKE_COLOR_BODY 7
#define SNAKE_COLOR_FOOD 2

#define SNAKE_GAMESTATE_COUNTDOWN 1
#define SNAKE_GAMESTATE_RUNNING 2
#define SNAKE_GAMESTATE_DEAD 3

#define SNAKE_GAME_SPEED 100  // millis per update, lower = faster.

class SnakeGame
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
    void draw_snake();
    void spawn_food();
    void move_snake();
    void update_direction();
    
    Display* _display;                        // Pointer to Display object.
    Controller* _controller;
    Menu* _menu;

    byte _game_state;
    
    unsigned long _score;
    unsigned long _score_last_update;
    
    byte _snake[200][2];                      // Snake body parts (head is at [_snake_length] index of array)
    byte _snake_direction = SNAKE_DIR_RIGHT;  // Direction in which snake is traveling.
    byte _snake_length;                       // Length of snake (number of body parts).
    unsigned long _game_last_update;          // Last time snake has moved.

    byte _food_x; // Location of food
    byte _food_y;
    bool _spawn_new_food = false; // Whether we need to spawn new food during the update.
    
    byte _countdown_state;
    unsigned long _countdown_last_update;     // Last time countdown state update was performed.

    boolean _dead_update_state;
    unsigned long _dead_last_update;
    unsigned long _dead_since;
};

#endif
