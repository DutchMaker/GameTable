#ifndef controller_h
#define controller_h

#include "Arduino.h"

// Shift register pins for lights (74HC595).
#define CONTROLLER_PIN_SRLIGHTS_LATCH 8
#define CONTROLLER_PIN_SRLIGHTS_CLOCK 7
#define CONTROLLER_PIN_SRLIGHTS_DATA  6

// Shift register pins for buttons (74HC165).
#define CONTROLLER_PIN_SRBUTTONS_LATCH        5
#define CONTROLLER_PIN_SRBUTTONS_CLOCK        3
#define CONTROLLER_PIN_SRBUTTONS_CLOCKENABLE  2
#define CONTROLLER_PIN_SRBUTTONS_DATA         4

// Bit positions for buttons and lights.
#define CONTROLLER_BIT_UP    3
#define CONTROLLER_BIT_DOWN  2
#define CONTROLLER_BIT_LEFT  1
#define CONTROLLER_BIT_RIGHT 0

// Light states.
#define CONTROLLER_LIGHT_STATE_OFF          0
#define CONTROLLER_LIGHT_STATE_BLINK_UP     1
#define CONTROLLER_LIGHT_STATE_BLINK_DOWN   2
#define CONTROLLER_LIGHT_STATE_BLINK_LEFT   3
#define CONTROLLER_LIGHT_STATE_BLINK_RIGHT  4
#define CONTROLLER_LIGHT_STATE_BLINK_ALL    5
#define CONTROLLER_LIGHT_STATE_TWIRL_SHORT  6
#define CONTROLLER_LIGHT_STATE_TWIRL_LONG   7

#define CONTROLLER_PLAYER1 0
#define CONTROLLER_PLAYER2 1

#define CONTROLLER_UPDATESPEED_BUTTONS  10
#define CONTROLLER_UPDATESPEED_LIGHTS   100

class Controller
{
  public:
    void    setup();
    void    update();
    void    set_light_state(uint8_t player, uint8_t state);
    int8_t  take_button_from_queue(uint8_t player);
    void    reset_queues();
    
  private:
    void    update_lights();
    void    update_buttons();
    uint8_t get_button_states();
    void    shrink_queue(uint8_t player);
    
    unsigned long _last_buttons_update = 0;
    unsigned long _last_lights_update = 0;

    uint8_t _last_buttons_data;
    uint8_t _queue[2][10];
    uint8_t _queue_length[2];

    uint8_t _player_light_state[2];
    bool    _player_light_blink_state[2];
    uint8_t _player_light_twirl_state[2];
};

#endif