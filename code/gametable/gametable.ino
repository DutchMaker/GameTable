#include "display.h"
#include "controller.h"
#include "snakegame.h"
#include "tetrisgame.h"
#include "ponggame.h"
#include "menu.h"
#include "countdown.h"

Display     display;
Controller  controller;
SnakeGame   snake_game;
TetrisGame  tetris_game;
PongGame    pong_game;
Countdown   countdown;
Menu        menu;

// TODO: 
//       - Tetris:  [IMP] Death screen: remove all lines one by one from the top
//       - Tetris:  [IMP] Change Tetris to play against framebuffer (save memory) NOT POSSIBLE...
//       - Tetris:  [NEW] Increase speed over time
//       - Menu:    [NEW] Player selection after boot
//       - General: [NEW] Rotate screen if player 2 is playing
//       - General: [IMP] Save RAM by moving data into single bytes?
//       - General: [IMP] add more comments?
//       - Snake:   [BUG] Sometimes more than one food spawns

void setup() 
{
  display.setup();
  controller.setup();
  countdown.setup(&display);

  snake_game.start(&display, &controller, &countdown, &menu);
  tetris_game.start(&display, &controller, &countdown, &menu);
  pong_game.start(&display, &controller, &countdown, &menu);
  menu.start(&display, &controller);
}

void loop() 
{
  controller.update();
  
  update_game();

  display.update();
}

void update_game()
{
  if (menu.run_game)
  {
    switch (menu.selected_game)
    {
      case 0:
        snake_game.update();
        break;
      case 1:
        tetris_game.update();
        break;
      case 2:
        pong_game.update();
        break;
    }
  }
  else
  {
    menu.update();
  }
}