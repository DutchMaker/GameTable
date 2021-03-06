#ifndef menu_h
#define menu_h

#include "Arduino.h"
#include "display.h"
#include "numericdisplay.h"
#include "controller.h"

#define MENU_NUM_GAMES 3
#define MENU_UPDATE_SPEED 150

class Menu
{
  public:
    void start(Display* display, NumericDisplay* numeric_displays, Controller* controller);
    void show_menu();
    void update();

    int8_t  selected_game;
    bool    run_game;

  private:
    void draw_options();
    void handle_input();
    void set_numeric_display_text();

    Display*        _display;
    NumericDisplay* _numeric_displays;
    Controller*     _controller;
    
    bool          _visible_state;
    uint8_t       _button_state;
    unsigned long _last_update;
};

#endif
