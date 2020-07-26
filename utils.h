// Common utilities.

#ifndef _MKIII_UTILS_H
#define _MKIII_UTILS_H

#include <Adafruit_NeoPixel.h>

inline uint32_t rgb32(uint8_t r, uint8_t g, uint8_t b) {
  return Adafruit_NeoPixel::Color(r, g, b);
}
inline uint32_t gamma32(uint32_t x) {
  return Adafruit_NeoPixel::gamma32(x);
}

const uint32_t c_expo = rgb32(30, 89, 174);
const uint32_t c_mill = rgb32(252, 208, 6);
const uint32_t c_bline = rgb32(243, 103, 23);
const uint32_t c_wce = rgb32(113, 30, 139);
const uint32_t c_cl = rgb32(0, 152, 201);
const uint32_t c_seabus = rgb32(130, 105, 94);

#endif
