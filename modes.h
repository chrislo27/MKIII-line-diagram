// Handles all the LED rendering in general operation.
// In the interest of space optimization, the implementation of the mode
// rendering is more C-like and doesn't leverage C++ features (like using a
// common abstract class w/ a virtual render function).

#ifndef _MKIII_MODES_H
#define _MKIII_MODES_H

#include <Adafruit_NeoPixel.h>
#include "diagram.h"

#define NUM_RENDER_MODES 2

typedef struct Mode0 {
  unsigned long lastTime = 99999;
  uint64_t lightPositions = 0L;
} Mode0;
extern Mode0 mode0;

void mode0_render(LineDiagram *diagram);
void mode0_renderStatic(LineDiagram *diagram);

typedef struct Mode1 {
  uint8_t submode = 0;
} Mode1;
extern Mode1 mode1;

void mode1_render(LineDiagram *diagram);


typedef struct Mode2 {
  uint8_t submode = 0;
  uint32_t strobe = 0;
  uint8_t lastCycle = 0;
} Mode2;
extern Mode2 mode2;

void mode2_render(LineDiagram *diagram);


#endif
