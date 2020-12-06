#ifndef FADE_TABLE_H
#define FADE_TABLE_H

#include "stdint.h"

enum fade_table {
  CIRCULAR_MOD,
  LINEAR_MOD,
  QUADRATIC
};

// interpolate the lookup table for different brightness levels
uint8_t get_brightness(enum fade_table table, uint8_t fade_step);

#endif
