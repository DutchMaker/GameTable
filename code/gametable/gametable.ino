#include "display.h"
#include "controller.h"
//#include "snakegame.h"
#include "tetrisgame.h"
#include "menu.h"

Display display;
Controller controller;
//SnakeGame snake_game;
TetrisGame tetris_game;
Menu menu;

// TODO: - Player selection after boot
//       - Rotate screen if player 2 is playing

void setup() 
{
  display.setup();
  controller.setup();

  //controller.set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_TWIRL_SHORT);
  //controller.set_light_state(CONTROLLER_PLAYER2, CONTROLLER_LIGHT_STATE_TWIRL_LONG);

  //snake_game.start(&led_display, &controller, &menu);
  tetris_game.start(&display, &controller, &menu);
  //menu.start(&led_display, &controller);
}

void loop() 
{
  controller.update();
  
  tetris_game.update();

  display.update();
}
