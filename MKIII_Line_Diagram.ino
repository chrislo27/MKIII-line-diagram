// Line diagram imitating the Vancouver SkyTrain Mark III train's
// LED real-time display. Designed for an Arduino Uno.

// IR remote controls:
// While the map is in operation, specific IR controls are non-functional
// (due to how updating the LEDs disrupts IR receiver timing). By pressing any
// remote button twice in 1000 ms, "IR mode" is enabled and the display is
// paused. To cancel, pressing the "FUNC/STOP" button exits out of "IR mode".
// In IR mode:
// - FUNC/STOP -> Exits IR mode.
// - POWER     -> Shuts off display and enters "sleep mode". Press POWER again to wake.
// - 0         -> Mode 0. Fading red.
// - 1         -> Mode 1. Press again to cycle through submodes. Static rendering (white, coloured).
// - 2         -> Mode 2. Press again to cycle through submodes. Colour test.
// - 3         -> Mode 3. Random line diagram.
// - 4         -> Mode 4. User-set line diagram.
// - 5         -> Mode 5. Custom set colour.
// While in Mode 4:
// - ST/REPT   -> Begin editing/exit editing.
// While in Mode 4, editing mode:
// - FFWD      -> Move endpoint by +1.
// - REWIND    -> Move endpoint by -1.
// - EQ        -> Swap the two slot positions.
// While in Mode 5:
// - ST/REPT   -> Begin editing/exit editing.
// While in Mode 5, editing mode:
// - num key   -> Add a digit for the current value.

#include <Adafruit_NeoPixel.h>
#include <Entropy.h>
#include <IRremote.h>
#include "ir_codes.h"
#include "stations.h"
#include "utils.h"
#include "modes.h"
#include "diagram.h"

#define IR_RECEIVER_PIN 3
IRrecv irrecv(IR_RECEIVER_PIN);
decode_results irresults;

#define LED_PIN 6
Adafruit_NeoPixel strip(NUM_STATIONS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define FRAME_DELAY 20

LineDiagram diagram(&strip);

struct {
  uint8_t clicks = 0;
  uint8_t clickTimer = 0;
  unsigned long timeSinceFirstClick = 0;
} IRMode;

struct {
  uint8_t currentMode = 1;
} Rendering;

// Call to update the LEDs based on the current mode.
void renderWithMode() {
  switch(Rendering.currentMode) {
    case 0:
      mode0_render(&diagram);
      break;
    case 1:
      mode1_render(&diagram);
      break;
    case 2:
      mode2_render(&diagram);
      break;
    case 3:
      mode3_render(&diagram);
      break;
    case 4:
      mode4_render(&diagram);
      break;
    case 5:
      mode5_render(&diagram);
      break;
    default:
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
      digitalWrite(LED_BUILTIN, HIGH);
      return;
  }
  strip.show();
}

// Call to update the LEDs in a NON-ANIMATING way, which should be uniquely representive of each mode.
// For some modes which are already static, their default render function may be called instead.
// This function clears the strip first.
void renderStaticWithMode() {
  strip.clear();
  bool didDefault = false;
  switch(Rendering.currentMode) {
    case 0:
      mode0_renderStatic(&diagram);
      break;
    case 2:
      mode2_renderStatic(&diagram);
      break;
    case 3:
      mode3_renderStatic(&diagram);
      break;
    case 4:
      mode4_renderStatic(&diagram);
      break;
    case 5:
      mode5_renderStatic(&diagram);
      break;
    default:
      renderWithMode();
      didDefault = true;
      break;
  }
  if (!didDefault) strip.show();
}

void handleIRMode(unsigned long value);
void animate(uint8_t id);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  Entropy.initialize();
  irrecv.enableIRIn();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

//  // B6, G#6, F6
//  tone(5, 1976);
//  delay(900); noTone(5); delay(100);
//  tone(5, 1661);
//  delay(900); noTone(5); delay(100);
//  tone(5, 1397);
//  delay(900); noTone(5);

  strip.begin();            // INITIALIZE strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels
  strip.setBrightness(70); // Set BRIGHTNESS (max = 255)
  randomSeed(Entropy.random());

  for (int i = 0; i < MODE2_PATTERN; i++)
    mode2.pattern[i] = i;
  mode2_shuffle();
  pathfind(&mode4.route, mode4.start, mode4.end);

//  StationPath result;
//  Serial.println(F("\nStation pathfind ordered test:"));
//  Serial.print(F("Broadway to 22nd:\n  "));
//  pathfind(&result, STN_BROADWAY, STN_22ND_STREET);
//  
//  for (int i = 0; i < result.size; i++) {
//    Serial.print(STN_NAMES[result.path[i]]);
//    Serial.print(F(", \n  "));
//  }
//  Serial.print(F("Done. Size: "));
//  Serial.println(result.size);
}

void loop() {
  if (IRMode.clicks < 2) {
    renderWithMode();
    delay(FRAME_DELAY); // 50 fps
    if (irrecv.decode(&irresults)) {
      if (IRMode.clickTimer == 0) {
        IRMode.clickTimer = 10;
        if (IRMode.clicks == 0) { // First click
          IRMode.clicks = 1;
          IRMode.timeSinceFirstClick = millis();
        } else {
          if (millis() - IRMode.timeSinceFirstClick < 1000) {
            // Switch to IR mode
            IRMode.clicks = 2;
            animate(1);
            renderStaticWithMode();
          } else { // Timed out for 2 clicks, reset to 1 click
            IRMode.clicks = 1;
            IRMode.timeSinceFirstClick = millis();
          }
        }
      }
      irrecv.resume();
    } else {
      if (IRMode.clickTimer > 0) IRMode.clickTimer--;
    }
  } else {
    delay(FRAME_DELAY);
    if (irrecv.decode(&irresults)) {
      handleIRMode(irresults.value);
      irrecv.resume();
    }
  }
}

void mode4_editMode();

void handleIRMode(unsigned long value) {
  switch (irresults.value) {
    case KEY_FUNC_STOP: // Exit IR mode
      IRMode.clicks = 0;
      animate(2);
      break;
    case KEY_POWER: // Sleep mode
      digitalWrite(LED_BUILTIN, LOW);
      animate(0);
      irrecv.resume();
      while (true) {
        delay(50);
        if (irrecv.decode(&irresults)) {
          if (irresults.value == KEY_POWER) break;
          irrecv.resume();
        }
      }
      irrecv.resume();
      digitalWrite(LED_BUILTIN, HIGH);
      animate(1);
      renderStaticWithMode();
      break;
    case KEY_0:
      Rendering.currentMode = 0;
      renderStaticWithMode();
      break;
    case KEY_1: {
      if (Rendering.currentMode != 1) {
        Rendering.currentMode = 1;
      } else {
        if (++mode1.submode >= 2) mode1.submode = 0;
      }
      renderStaticWithMode();
      break;
    }
    case KEY_2:
      if (Rendering.currentMode != 2) {
        Rendering.currentMode = 2;
      } else {
        if (++mode2.submode >= 2) mode2.submode = 0;
      }
      renderStaticWithMode();
      break;
    case KEY_3:
      Rendering.currentMode = 3;
      renderStaticWithMode();
      break;
    case KEY_4:
      Rendering.currentMode = 4;
      renderStaticWithMode();
      break;
    case KEY_5:
      Rendering.currentMode = 5;
      renderStaticWithMode();
      break;
    case KEY_ST_REPT: {
      if (Rendering.currentMode == 4) {
        mode4_editMode();
      } else if (Rendering.currentMode == 5) {
        mode5_editMode();
      }
      break;
    }
  }
}

void mode4_editMode() {
  mode4_render(&diagram, true);
  strip.show();
  irrecv.resume();
  while (true) {
    delay(FRAME_DELAY);
    if (irrecv.decode(&irresults)) {
      unsigned long value = irresults.value;
      switch (value) {
        case KEY_ST_REPT: {
          irrecv.resume();
          renderStaticWithMode();
          return;
        }
        case KEY_EQ: {
          StationPath *route = &mode4.route;
          uint8_t first = mode4.start;
          uint8_t last = mode4.end;
          mode4.start = last;
          mode4.end = first;
          pathfind(&mode4.route, last, first);
          mode4_render(&diagram, true);
          strip.show();
          break;
        }
        case KEY_REWIND:
        case KEY_FAST_FORWARD: {
          int8_t last = (int8_t) mode4.end;
          last += value == KEY_REWIND ? -1 : 1;
          if (last < 0) last = NUM_STATIONS - 1;
          if (last >= NUM_STATIONS) last = 0;
          mode4.end = (uint8_t) last;
          pathfind(&mode4.route, mode4.start, mode4.end);
          mode4_render(&diagram, true);
          strip.show();
          break;
        }
      }
      irrecv.resume();
    }
  }
}

void mode5_bumpUp(uint8_t digit, uint16_t* rawValue) {
  uint8_t steps = mode5.steps;
  uint8_t substep = steps % 3;
  
  (*rawValue) *= 10;
  (*rawValue) += digit % 10;
  if ((*rawValue) > 255) (*rawValue) = 255;
  if (steps / 3 == 0) {
    mode5.red = (uint8_t) (*rawValue);
  } else if (steps / 3 == 1) {
    mode5.green = (uint8_t) (*rawValue);
  } else if (steps / 3 == 2) {
    mode5.blue = (uint8_t) (*rawValue);
  }

  if (substep == 2) (*rawValue) = 0;
  mode5.steps++;
  mode5_render(&diagram, true, false);
  strip.show();
}

void mode5_editMode() {
  uint8_t oldRed = mode5.red;
  uint8_t oldGreen = mode5.green;
  uint8_t oldBlue = mode5.blue;
  mode5.steps = 0;
  mode5_render(&diagram, true, false);
  strip.show();
  mode5.red = mode5.blue = mode5.green = 0;
  irrecv.resume();
  uint16_t rawValue = 0;
  while (true) {
    delay(FRAME_DELAY);
    if (irrecv.decode(&irresults)) {
      unsigned long value = irresults.value;
      switch (value) {
        case KEY_ST_REPT: {
          irrecv.resume();
          mode5.red = oldRed;
          mode5.green = oldGreen;
          mode5.blue = oldBlue;
          renderStaticWithMode();
          return;
        }
        case KEY_0:
          mode5_bumpUp(0, &rawValue);
          break;
        case KEY_1:
          mode5_bumpUp(1, &rawValue);
          break;
        case KEY_2:
          mode5_bumpUp(2, &rawValue);
          break;
        case KEY_3:
          mode5_bumpUp(3, &rawValue);
          break;
        case KEY_4:
          mode5_bumpUp(4, &rawValue);
          break;
        case KEY_5:
          mode5_bumpUp(5, &rawValue);
          break;
        case KEY_6:
          mode5_bumpUp(6, &rawValue);
          break;
        case KEY_7:
          mode5_bumpUp(7, &rawValue);
          break;
        case KEY_8:
          mode5_bumpUp(8, &rawValue);
          break;
        case KEY_9:
          mode5_bumpUp(9, &rawValue);
          break;
      }
      irrecv.resume();
      if (mode5.steps >= 9) {
        mode5.steps = 0;
        renderStaticWithMode();
        return;
      }
    }
  }
}

/*
 * 0 - Turn all pixels off
 * 1 - IR mode on / resume from sleep
 * 2 - IR mode off (reverse of 1)
 */
void animate(uint8_t id) {
  switch(id) {
    case 0:
      strip.clear();
      strip.show();
      break;
    case 1:
    case 2:
      strip.clear();
      for (int i = 0; i < NUM_STATIONS; i++) {
        for (int j = i; j >= 0; j--) {
          diagram.set(STATION_X_ORDER[id == 2 ? (NUM_STATIONS - 1 - j) : j], strip.ColorHSV(65536 / NUM_STATIONS * (j - i)));
        }
        strip.show();
        delay(7);
      }
      for (int i = 0; i < NUM_STATIONS; i++) {
        for (int j = 0; j < NUM_STATIONS; j++) {
          uint32_t c = j <= i ? 0 : strip.ColorHSV(65536 / NUM_STATIONS * (j - i));
          diagram.set(STATION_X_ORDER[id == 2 ? (NUM_STATIONS - 1 - j) : j], c);
        }
        strip.show();
        delay(7);
      }
      break;
  }
}
