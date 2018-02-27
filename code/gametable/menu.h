#ifndef menu_h
#define menu_h

#include "Arduino.h"
#include "display.h"
#include "controller.h"
#include "point.h"

#define MENU_UPDATE_SPEED 300

class Menu
{
  public:
    void start(Display* display, Controller* controller);
    void show_menu();
    void update();

    int8_t selected_game;
    bool run_game;

  private:
    void draw_options();
    void handle_input();

    Display* _display;                        // Pointer to Display object.
    Controller* _controller;

    bool _visible_state;
    unsigned long _last_update;
};

#endif
