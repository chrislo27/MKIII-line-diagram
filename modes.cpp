#include <Adafruit_NeoPixel.h>
#include <Entropy.h>
#include "modes.h"
#include "stations.h"
#include "diagram.h"

// -------------------------- Mode 0 --------------------------
// Randomly picks some station LEDs to fade in/out red.
// Only supports up to the first 64 station LEDs.
// https://youtu.be/amxVBWTSp2o?t=136

Mode0 mode0;

void mode0_render(LineDiagram *diagram) {
  const unsigned long animationTime = 4000;
  const unsigned long halfAniTime = animationTime / 2;
  long timeDiff = millis() - mode0.lastTime;
  Adafruit_NeoPixel *strip = diagram->strip;
  if (timeDiff >= animationTime) {
    mode0.lastTime = millis();
    uint64_t lightPositions = 0L;
    lightPositions |= (Entropy.random() & 0xFFFFFFFF);
    lightPositions <<= 32;
    lightPositions |= (Entropy.random() & 0xFFFFFFFF);
    mode0.lightPositions = lightPositions;
    strip->clear();
  } else {
    int16_t red = timeDiff > halfAniTime ? map(timeDiff - halfAniTime, 0, halfAniTime, 255, 0) : map(timeDiff, 0, halfAniTime, 0, 255);
    uint32_t newColor = rgb32((uint8_t) red, 0, 0);
    for (int i = 0; i < min(NUM_STATIONS, 64); i++) {
      if (((mode0.lightPositions >> i) & 1) == 1) {
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
// Static rendering for testing.
// Submode 0 - Sets all LEDs to #7F7F7F.
// Submode 1 - Sets all stations to their appropriate line colours.

Mode1 mode1;

void mode1_render(LineDiagram *diagram) {
  diagram->strip->clear();
  switch (mode1.submode) {
    case 0: {
      uint32_t color = rgb32(127, 127, 127);
      for (int i = 0; i < diagram->strip->numPixels(); i++) {
        diagram->set(i, color, false);
      }
      break;
    }
    case 1: {
      for (int i = 0; i < NUM_STATIONS; i++) {
        diagram->set(i, (i < 20 || i == STN_BRAID || i == STN_SAPPERTON) ? c_expo : c_mill);
      }
      break;
    }
  }
}


// -------------------------- Mode 2 --------------------------
// Colour test. Cycles through the rainbow slowly for all LEDs.

void mode2_render(LineDiagram *diagram) {
  Adafruit_NeoPixel *strip = diagram->strip;
  const uint16_t num = strip->numPixels();
  const uint16_t cycle = map(millis() % 5000, 0, 5000, 0, 65535);
  strip->clear();
  for (int i = 0; i < num; i++) {
    diagram->set(i, strip->ColorHSV(65536 / num * (num - i - 1) + cycle));
  }
}
