#ifndef point_h
#define point_h

#include "Arduino.h"

struct Point
{
  int8_t x;
  int8_t y;
};

Point get_point(int8_t x, int8_t y);

#endif
