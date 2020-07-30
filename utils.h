// Common utilities.

#ifndef _MKIII_UTILS_H
#define _MKIII_UTILS_H

#include <Adafruit_NeoPixel.h>

#define rgb32(r, g, b) (((uint32_t) (r) << 16) | ((uint32_t) (g) <<  8) | (b))

inline uint32_t gamma32(uint32_t x) {
  return Adafruit_NeoPixel::gamma32(x);
}

const uint32_t c_stn_green = rgb32(0, 127, 0);
const uint32_t c_stn_red = rgb32(127, 0, 0);
const uint32_t c_stn_yellow = rgb32(252, 208, 6);
const uint32_t c_stn_purple = rgb32(113, 30, 139);
const uint32_t c_stn_orange = rgb32(243, 103, 23);
const uint32_t c_stn_blue = rgb32(30, 89, 174);

const uint32_t c_expo = rgb32(30, 89, 174);
const uint32_t c_mill = rgb32(252, 208, 6);
const uint32_t c_bline = rgb32(243, 103, 23);
const uint32_t c_wce = rgb32(113, 30, 139);
const uint32_t c_cl = rgb32(0, 152, 201);
const uint32_t c_seabus = rgb32(130, 105, 94);

#endif
