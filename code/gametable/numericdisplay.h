#ifndef numericdisplay_h
#define numericdisplay_h

#include "Arduino.h"
#include "DigitLedDisplay.h"
#include "controller.h"

#define NUMERICDISPLAY_BRIGHTNESS 10
#define NUMERICDISPLAY_NUM_DIGITS 8

#define NUMERICDISPLAY_PLAYER1_PIN_DIN 10
#define NUMERICDISPLAY_PLAYER1_PIN_CS  11
#define NUMERICDISPLAY_PLAYER1_PIN_CLK 12

#define NUMERICDISPLAY_PLAYER2_PIN_DIN A0
#define NUMERICDISPLAY_PLAYER2_PIN_CS  A1
#define NUMERICDISPLAY_PLAYER2_PIN_CLK A2

#define NUMERICDISPLAY_CHARACTER(index) (uint8_t)(pgm_read_byte(&characters[index]))

const static uint8_t characters [] PROGMEM = {
    B01110111,  // A
    B01111111,  // B
    B01001110,  // C
    B01111110,  // D
    B01001111,  // E
    B01000111,  // F
    B01011110,  // G
    B00110111,  // H
    B00110000,  // I
    B00111000,  // J
    B00000000,  // K
    B00001110,  // L
    B00000000,  // M
    B00000000,  // N
    B01111110,  // O
    B01100111,  // P
    B00000000,  // Q
    B01000110,  // R
    B01011011,  // S
    B01000111,  // T
    B01111010,  // U
    B01111010,  // V
    B00000000,  // W
    B00000000,  // X
    B00110011,  // Y  
    B00000000,  // Z
};

class NumericDisplay
{
  public:
    void setup();
    void set_value(long value, uint8_t player);
    void set_values(long value);
    void off();
    void on();
    void write(uint8_t player, uint8_t digit, char character);

  private:
    DigitLedDisplay _display_player1 = DigitLedDisplay(NUMERICDISPLAY_PLAYER1_PIN_DIN, NUMERICDISPLAY_PLAYER1_PIN_CS, NUMERICDISPLAY_PLAYER1_PIN_CLK);
    DigitLedDisplay _display_player2 = DigitLedDisplay(NUMERICDISPLAY_PLAYER2_PIN_DIN, NUMERICDISPLAY_PLAYER2_PIN_CS, NUMERICDISPLAY_PLAYER2_PIN_CLK);
};

#endif