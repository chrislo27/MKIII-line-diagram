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
// - 0         -> Mode 0.
// - 1         -> Mode 1.
// - 2         -> Mode 2.

// WIRING BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between LED strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - LED strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting LEDs on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.

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

  strip.begin();           // INITIALIZE strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(64); // Set BRIGHTNESS (max = 255)
  randomSeed(Entropy.random());
}

void loop() {
  if (IRMode.clicks < 2) {
    renderWithMode();
    delay(20); // 50 fps
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
    delay(20);
    if (irrecv.decode(&irresults)) {
      handleIRMode(irresults.value);
      irrecv.resume();
    }
  }
}

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
      Rendering.currentMode = 2;
      renderStaticWithMode();
      break;
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
