#include "numericdisplay.h"

void NumericDisplay::setup()
{
  _display_player1.setBright(NUMERICDISPLAY_BRIGHTNESS);
  _display_player1.setDigitLimit(NUMERICDISPLAY_NUM_DIGITS);

  _display_player2.setBright(NUMERICDISPLAY_BRIGHTNESS);
  _display_player2.setDigitLimit(NUMERICDISPLAY_NUM_DIGITS);
}

void NumericDisplay::set_value(long value, uint8_t player)
{
  if (player == CONTROLLER_PLAYER1)
  {
    _display_player1.printDigit(value);
  }
  else if (player == CONTROLLER_PLAYER2)
  {
    //_display_player2.printDigit(value);
  }
}

void NumericDisplay::set_values(long value)
{
  set_value(value, CONTROLLER_PLAYER1);
  set_value(value, CONTROLLER_PLAYER2);
}

void NumericDisplay::off()
{
  _display_player1.off();
  //_display_player2.off();
}

void NumericDisplay::on()
{
  _display_player1.on();
  _display_player1.clear();

  //_display_player2.on();
  //_display_player2.clear();
}

void NumericDisplay::write(uint8_t digit, char character)
{
  _display_player1.write(digit, NUMERICDISPLAY_CHARACTER(((uint8_t)character) - 65));
}