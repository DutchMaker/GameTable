#include "controller.h"

void Controller::setup()
{
  // Setup lights shift register.
  pinMode(CONTROLLER_PIN_SRLIGHTS_LATCH, OUTPUT);
  pinMode(CONTROLLER_PIN_SRLIGHTS_CLOCK, OUTPUT);
  pinMode(CONTROLLER_PIN_SRLIGHTS_DATA, OUTPUT);

  // Setup buttons shift register.
  pinMode(CONTROLLER_PIN_SRBUTTONS_CLOCKENABLE, OUTPUT);
  pinMode(CONTROLLER_PIN_SRBUTTONS_CLOCK, OUTPUT);
  pinMode(CONTROLLER_PIN_SRBUTTONS_DATA, INPUT);
  pinMode(CONTROLLER_PIN_SRBUTTONS_LATCH, OUTPUT);

  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCKENABLE, HIGH);
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCK, LOW);
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_LATCH, LOW);

  // Initialize variables.
  _last_lights_update = 0;
  _last_buttons_update = 0;

  reset_queues();
}

void Controller::update()
{
  update_lights();
  update_buttons();
}

///
/// Buttons logic
///

void Controller::update_buttons()
{
  if (millis() - _last_buttons_update < CONTROLLER_UPDATESPEED_BUTTONS)
  {
    return;
  }
  
  // Get buttons state and ensure it has changed since the last update.
  uint8_t data = get_button_states();

  if (data == _last_buttons_data)
  {
    _last_buttons_update = millis();
    return;
  }

  // Handle the button state for all players.
  for (uint8_t player = 0; player < 2; player++)
  {
    if (_queue_length[player] >= 9)
    {
      // Button queue is full.
      continue;
    }

    uint8_t bitshift = 4 - (player * 4);  // Player 1 buttons are bits 4 to 7
                                          // Player 2 buttons are bits 0 to 3

    // Read button states.
    bool up = bitRead(data, CONTROLLER_PLAYER_BIT_UP + bitshift) == 0;
    bool down = bitRead(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift) == 0;
    bool left = bitRead(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift) == 0;
    bool right = bitRead(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift) == 0;

    // TODO: Check last button state PER BUTTON.

    // Add pressed buttons to queue.
    if (up && (_queue_length[player] == 0 || _queue[player][_queue_length[player] - 1] != CONTROLLER_PLAYER_BIT_UP))
    {
      _queue[player][_queue_length[player]++] = CONTROLLER_PLAYER_BIT_UP;
    }

    if (down && (_queue_length[player] == 0 || _queue[player][_queue_length[player] - 1] != CONTROLLER_PLAYER_BIT_DOWN))
    {
      _queue[player][_queue_length[player]++] = CONTROLLER_PLAYER_BIT_DOWN;
    }

    if (left && (_queue_length[player] == 0 || _queue[player][_queue_length[player] - 1] != CONTROLLER_PLAYER_BIT_LEFT))
    {
      _queue[player][_queue_length[player]++] = CONTROLLER_PLAYER_BIT_LEFT;
    }

    if (right && (_queue_length[player] == 0 || _queue[player][_queue_length[player] - 1] != CONTROLLER_PLAYER_BIT_RIGHT))
    {
      _queue[player][_queue_length[player]++] = CONTROLLER_PLAYER_BIT_RIGHT;
    }
  }

  _last_buttons_data = data;
  _last_buttons_update = millis();
}

// Return the next button in the queue and remove it from the queue.
// Returns -1 when no buttons was pressed!
int8_t Controller::take_button_from_queue(uint8_t player)
{
  if (_queue_length[player] == 0)
  {
    return -1;
  }
  
  uint8_t button = _queue[player][0];
  shrink_queue(player);

  return button;
}

// Remove first in line from the queue.
void Controller::shrink_queue(uint8_t player)
{
  for (uint8_t i = 1; i < 10; i++)
  {
    _queue[player][i - 1] = _queue[player][i];
  }

  _queue[player][9] = 0;

  if (_queue_length[player] > 0)
  {
    _queue_length[player]--;
  }
}

void Controller::reset_queues()
{
  for (uint8_t player = 0; player < 2; player++)
  {
    for (uint8_t i = 0; i < 10; i++)
    {
      _queue[player][i] = 0;
    }

    _queue_length[player] = 0;
    _last_buttons_data = 0;
  }
}

uint8_t Controller::get_button_states()
{
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_LATCH, LOW);
  delayMicroseconds(5);
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_LATCH, HIGH);
  delayMicroseconds(5);

  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCK, HIGH);
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCKENABLE, LOW);
  
  uint8_t data = shiftIn(CONTROLLER_PIN_SRBUTTONS_DATA, CONTROLLER_PIN_SRBUTTONS_CLOCK, LSBFIRST);

  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCKENABLE, HIGH);
  digitalWrite(CONTROLLER_PIN_SRBUTTONS_CLOCK, LOW);

  return data;
}

///
/// Lights logic
///

// Set the light state for a specific player.
void Controller::set_light_state(uint8_t player, uint8_t state)
{
  uint8_t index = player == CONTROLLER_PLAYER1
    ? 0
    : 1;

  _player_light_state[index] = state;
  _player_light_blink_state[index] = false;
  _player_light_twirl_state[index] = 0;
}

// Update loop for lights.
void Controller::update_lights()
{
  if (millis() - _last_lights_update < CONTROLLER_UPDATESPEED_LIGHTS)
  {
    return;
  }

  uint8_t data = 0;

  // Handle light states for all players.
  for (uint8_t player = 0; player < 2; player++)
  {
    uint8_t bitshift = 4 - (player * 4);  // Player 1 buttons are bits 4 to 7
                                          // Player 2 buttons are bits 0 to 3
    
    // Handle BLINK state.
    if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_ALL ||
      _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_UP ||
      _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_DOWN ||
      _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_LEFT ||
      _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_RIGHT)
    {
      if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_UP || _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_ALL)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, (_player_light_blink_state[player] ? 0 : 1));
      }

      if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_DOWN || _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_ALL)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, (_player_light_blink_state[player] ? 0 : 1));
      }

      if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_LEFT || _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_ALL)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, (_player_light_blink_state[player] ? 0 : 1));
      }

      if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_RIGHT || _player_light_state[player] == CONTROLLER_LIGHT_STATE_BLINK_ALL)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, (_player_light_blink_state[player] ? 0 : 1));
      }

      _player_light_blink_state[player] = !_player_light_blink_state[player];
    }

    // Handle TWIRL_SHORT state (turn lights on/off clockwise).
    if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_TWIRL_SHORT)
    {
      if (_player_light_twirl_state[player] == 0)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 0);
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 0);
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 0);
      }
      else if (_player_light_twirl_state[player] == 1)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 0);
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 2)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 0);
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 3)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 0);
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 1);
      }
    
      if (++_player_light_twirl_state[player] > 3)
      {
        _player_light_twirl_state[player] = 0;
      }
    }

    // Handle TWIRL_LONG state (turn lights on/off clockwise, all on at same time).
    if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_TWIRL_LONG)
    {
      if (_player_light_twirl_state[player] == 0)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 1)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 2)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 3)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 4)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 5)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 1);
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 1);
      }
      else if (_player_light_twirl_state[player] == 6)
      {
        bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 1);
      }
      
      if (++_player_light_twirl_state[player] > 7)
      {
        _player_light_twirl_state[player] = 0;
      }
    }

    // Handle OFF state.
    if (_player_light_state[player] == CONTROLLER_LIGHT_STATE_OFF)
    {
      bitWrite(data, CONTROLLER_PLAYER_BIT_UP + bitshift, 0);
      bitWrite(data, CONTROLLER_PLAYER_BIT_DOWN + bitshift, 0);
      bitWrite(data, CONTROLLER_PLAYER_BIT_LEFT + bitshift, 0);
      bitWrite(data, CONTROLLER_PLAYER_BIT_RIGHT + bitshift, 0);
    }
  }

  // Shift out the lights data.
  digitalWrite(CONTROLLER_PIN_SRLIGHTS_LATCH, LOW);
  shiftOut(CONTROLLER_PIN_SRLIGHTS_DATA, CONTROLLER_PIN_SRLIGHTS_CLOCK, LSBFIRST, data);
  digitalWrite(CONTROLLER_PIN_SRLIGHTS_LATCH, HIGH);

  _last_lights_update = millis();
}