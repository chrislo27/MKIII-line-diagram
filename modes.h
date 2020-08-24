// Handles all the LED rendering in general operation.
// In the interest of space optimization, the implementation of the mode
// rendering is more C-like and doesn't leverage C++ features (like using a
// common abstract class w/ a virtual render function).

#ifndef _MKIII_MODES_H
#define _MKIII_MODES_H

#include <Adafruit_NeoPixel.h>
#include "diagram.h"
#include "stations.h"

#define NUM_RENDER_MODES 6
#define MODE2_PATTERN 16

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
  uint8_t lastCycle = 0;
  uint8_t pattern[MODE2_PATTERN];
} Mode2;
extern Mode2 mode2;

void mode2_render(LineDiagram *diagram);
void mode2_renderStatic(LineDiagram *diagram);
void mode2_shuffle();


typedef struct Mode3 {
  StationPath route;
  uint8_t originalSize = 0;
  unsigned long lastTime = 0L;
} Mode3;
extern Mode3 mode3;

void mode3_render(LineDiagram *diagram);
void mode3_renderStatic(LineDiagram *diagram);


typedef struct Mode4 {
  StationPath route;
  uint8_t start = STN_VCC_CLARK;
  uint8_t end = STN_LAFARGE;
} Mode4;
extern Mode4 mode4;

void mode4_render(LineDiagram *diagram);
void mode4_render(LineDiagram *diagram, bool editMode);
void mode4_renderStatic(LineDiagram *diagram);


typedef struct Mode5 {
  uint8_t red = 255;
  uint8_t green = 255;
  uint8_t blue = 255;
  uint8_t steps = 0;
} Mode5;
extern Mode5 mode5;

void mode5_render(LineDiagram *diagram);
void mode5_render(LineDiagram *diagram, bool editMode, bool noSteps);
void mode5_renderStatic(LineDiagram *diagram);

#endif
