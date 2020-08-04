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
  Adafruit_NeoPixel *strip = diagram->strip;
  strip->clear();
  switch (mode1.submode) {
    case 0: {
      uint32_t color = rgb32(127, 127, 127);
      for (int i = 0; i < strip->numPixels(); i++) {
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
// Colour test.
// Submode 1 - Cycles through the rainbow slowly for all LEDs.
// Submode 2 - Patterned strobing ("rave").

Mode2 mode2;

void mode2_render(LineDiagram *diagram, unsigned long ms) {
  Adafruit_NeoPixel *strip = diagram->strip;
  const uint16_t num = strip->numPixels();
  strip->clear();
  switch (mode2.submode) {
    case 0: {
      const uint16_t cycle = map(ms % 5000, 0, 5000, 0, 65535);
      for (int i = 0; i < num; i++) {
        diagram->set(i, strip->ColorHSV(65536 / num * (num - i - 1) + cycle));
      }
      break;
    }
    case 1: {
      uint8_t cycle = (ms / 250 % 32);
      if (cycle < 16) cycle /= 4;
      else if (cycle >= 16 && cycle < 24) cycle = (cycle - 16) / 2;
      else cycle = cycle - 24;
      if (cycle != mode2.lastCycle) {
        mode2.lastCycle = cycle;
        mode2.strobe = strip->ColorHSV(65536 / 48 * random(48));
      }
      const uint16_t num = strip->numPixels();
      strip->clear();
      const uint32_t strobe = mode2.strobe;
      for (int i = 0; i < num; i++) {
        diagram->set(i, strobe);
      }
      break;
    }
  }
}
void mode2_render(LineDiagram *diagram) {
  mode2_render(diagram, millis());
}

void mode2_renderStatic(LineDiagram *diagram) {
  mode2_render(diagram, 500);
}


// -------------------------- Mode 3 --------------------------
// Random line diagram. A random path will be picked
// and "travelled" to.

Mode3 mode3;

void mode3_render(LineDiagram *diagram) {
  StationPath *route = &(mode3.route);
  if (route->size == 0) {
    // Regenerate the route
    uint8_t first, second;
    while (route->size == 0) {
      first = random(0, NUM_STATIONS);
      do {
        second = random(0, NUM_STATIONS);
      } while (second == first);
      pathfind(route, first, second);
    }
    mode3.originalSize = route->size;
    mode3.lastTime = millis();
  }
  // Display path
  diagram->strip->clear();
  for (int i = 0; i < route->size; i++) {
    diagram->set(route->path[i], i == 0 ? c_stn_red : c_stn_green);
  }
  if (millis() - mode3.lastTime > 750) {
    mode3.lastTime = millis();
    route->size--;
  }
}

void mode3_renderStatic(LineDiagram *diagram) {
  for (int i = STN_BURRARD; i <= STN_COLUMBIA; i++) {
    diagram->set(i, c_stn_green);
  }
  diagram->set(STN_BRAID, c_stn_green);
  diagram->set(STN_SAPPERTON, c_stn_green);
  diagram->set(STN_PRODUCTION, c_stn_green);
  diagram->set(STN_LOUGHEED, c_stn_green);
  diagram->set(STN_WATERFRONT, c_stn_red);
}


// -------------------------- Mode 4 --------------------------
// Custom line diagram. Set your own static path.

Mode4 mode4;

void mode4_render(LineDiagram *diagram) {
  mode4_render(diagram, false);
}

void mode4_render(LineDiagram *diagram, bool editMode) {
  diagram->strip->clear();
  StationPath *route = &mode4.route;
  if (route->size == 0) {
    // Path is invalid so highlight orange/purple
    diagram->set(mode4.start, c_stn_orange);
    diagram->set(mode4.end, c_stn_purple);
  } else {
    for (int i = 0; i < route->size; i++) {
      diagram->set(route->path[i], i == 0 ? c_stn_red : (!editMode || i == route->size - 1 ? c_stn_green : c_stn_yellow));
    }
  }
}

void mode4_renderStatic(LineDiagram *diagram) {
  for (int i = STN_LAFARGE; i <= STN_VCC_CLARK; i++) {
    diagram->set(i, c_stn_green);
  }
  diagram->set(STN_BRAID, 0);
  diagram->set(STN_SAPPERTON, 0);
  diagram->set(STN_VCC_CLARK, c_stn_red);
}
