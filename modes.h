// Handles all the LED rendering in general operation.
// In the interest of space optimization, the implementation of the mode
// rendering is C-like and doesn't leverage C++ features (like using a
// common abstract class w/ a virtual render function).

#ifndef _MKIII_MODES_H
#define _MKIII_MODES_H

#include <Adafruit_NeoPixel.h>
#include "diagram.h"

#define NUM_RENDER_MODES 2

void mode0_render(LineDiagram *diagram);
void mode1_render(LineDiagram *diagram);

void mode0_renderStatic(LineDiagram *diagram);

#endif
