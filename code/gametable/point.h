#ifndef point_h
#define point_h

#include "Arduino.h"

struct Point
{
  byte x;
  byte y;
};

Point get_point(byte x, byte y);

#endif
