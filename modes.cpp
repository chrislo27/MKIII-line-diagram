#include <Adafruit_NeoPixel.h>
#include <Entropy.h>
#include "modes.h"
#include "stations.h"
#include "diagram.h"

// -------------------------- Mode 0 --------------------------
// Randomly picks some station LEDs to fade in/out red.
// Only supports up to the first 64 station LEDs.
// https://youtu.be/amxVBWTSp2o?t=136

struct {
  unsigned long lastTime = 99999;
  uint64_t lightPositions = 0L;
} Mode0;

void mode0_render(LineDiagram *diagram) {
  const unsigned long animationTime = 4000;
  const unsigned long halfAniTime = animationTime / 2;
  long timeDiff = millis() - Mode0.lastTime;
  Adafruit_NeoPixel *strip = diagram->strip;
  if (timeDiff >= animationTime) {
    Mode0.lastTime = millis();
    uint64_t lightPositions = 0L;
    lightPositions |= (Entropy.random() & 0xFFFFFFFF);
    lightPositions <<= 32;
    lightPositions |= (Entropy.random() & 0xFFFFFFFF);
    Mode0.lightPositions = lightPositions;
    strip->clear();
  } else {
    int16_t red = (int16_t) (timeDiff > halfAniTime ? (255 - (255 * (timeDiff - halfAniTime) / (float) halfAniTime)) : (255 * timeDiff / (float) halfAniTime));
    if (red < 1) red = 1;
    if (red > 255) red = 255;
    uint32_t newColor = rgb32((uint8_t) red, 0, 0);
    for (int i = 0; i < min(NUM_STATIONS, 64); i++) {
      if (((Mode0.lightPositions >> i) & 1) == 1) {
        diagram->set(i, newColor);
      }
    }
  }
}

void mode0_renderStatic(LineDiagram *diagram) {
  uint32_t red = rgb32(130, 0, 0);
  for (int i = 0; i < min(NUM_STATIONS, 64); i += 2) {
    diagram->set(i, red);
  }
}

// -------------------------- Mode 1 --------------------------
// Sets all LEDs to #7F7F7F.

void mode1_render(LineDiagram *diagram) {
  uint32_t color = rgb32(127, 127, 127);
  for (int i = 0; i < diagram->strip->numPixels(); i++) {
    diagram->set(i, color, false);
  }
}
