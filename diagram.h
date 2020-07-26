// Interface for interacting with the LEDs on the physical line diagram.
// This class can account for things such as automatic gamma correction,
// tweaking the brightness of individual pixels if necessary, etc.

#ifndef _MKIII_DIAGRAM_H
#define _MKIII_DIAGRAM_H

#include <Adafruit_NeoPixel.h>
#include "stations.h"
#include "utils.h"

class LineDiagram {
  public:
    Adafruit_NeoPixel *strip;
    
    LineDiagram(Adafruit_NeoPixel *strip);
    // Set the color for the particular station number
    void set(uint16_t stn, uint32_t color, bool gamma = true);
    
};

#endif
