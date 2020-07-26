#include <Adafruit_NeoPixel.h>
#include "diagram.h"
#include "stations.h"
#include "utils.h"

LineDiagram::LineDiagram(Adafruit_NeoPixel *strip) {
  this->strip = strip;
}

void LineDiagram::set(uint16_t stn, uint32_t color, bool gamma) {
  strip->setPixelColor(stn, gamma ? gamma32(color) : color);
}
