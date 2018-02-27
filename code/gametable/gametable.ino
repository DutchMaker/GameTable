#include "controller.h"

Controller controller;

void setup() 
{
  Serial.begin(9600);
  
  controller.setup();
  controller.set_light_state(CONTROLLER_PLAYER1, CONTROLLER_LIGHT_STATE_TWIRL_LONG);
}

void loop() 
{
  controller.update();
}