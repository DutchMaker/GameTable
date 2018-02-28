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

// TODO: 
//       - Test:    CODE check, randomize on lin 289 tetrisgame
//       - Test:    Added reset_queues for left/right in Tetris (maybe increase TETRIS_MOVE_SPEED)
//       - Test:    Better random pieces (never repeat more than twice)
//       - Test:    Tetris Scoring with multiplier (remove serial print after test!)
//
//       - Test:    Different display brightness
//       - Test:    Change brightness for one pixel
//       - Menu:    Change menu to vertical selection and add Pong
//       - Menu:    Player selection after boot
//       - Tetris:  Change Tetris to play against framebuffer (save memory)
//       - Tetris:  Don't generate same tetronimo 3 times
//       - General: Rotate screen if player 2 is playing
//       - General: Move countdown logic to separate class
//       - General: Move constant arrays to PROGMEM (https://www.arduino.cc/reference/en/language/variables/utilities/progmem/)
//                  Save RAM by moving data into single bytes?
//       - General: add more comments?

void setup() 
{
  Serial.begin(9600);         // TODO: Remove when score is tested
  randomSeed(analogRead(0));  // TODO: Remove when menu is implemented

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
  /*controller.update();
  
  tetris_game.update();

  display.update();*/

  color_demo();
}

void update_game()
{
  if (menu.run_game)
  {
    switch (menu.selected_game)
    {
      case 0:
        //snake_game.update();
        break;
      case 1:
        tetris_game.update();
        break;
      case 2:
        //pong_game.update();
        break;
    }
  }
  else
  {
    menu.update();
  }
}

void color_demo()
{
  for (int i = 0; i < 12; i++)
  {
    display.set_pixel(i, 0, i);
  }

  display.update();

  delay(1000);
}