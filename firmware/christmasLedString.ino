/* ----- Christmas Light Controller for ATtiny85 © 2026 Robert de Beer -----

    This ATtiny85 sketch creates a smooth, breathing light effect for Christmas lights.
    It gently fades LEDs in and out using a natural Gaussian curve, with two modes: 
      - standard (5-second cycles)
      - random (1-20 second varied timing). 
    Between each fade cycle, the lights rest at a warm ember glow, creating  a gentle 
    breathing/pulsing effect perfect for holiday decorations.
    The Gaussian curve creates a natural, organic fade—gentle slow rise and fall—that 
    mimics breathing, matching the eye’s logarithmic brightness perception to prevent 
    long bright periods and short off-times from feeling artificial.

    The string consists of 20 leds, 10 red, 10 white all parallel ( premade cheap Chinese).
    To drive them safely:
      LED_PIN → Resistor ( 22 ohm) → Gate of N-Channel MOSFET
      MOSFET Source → GND
      MOSFET Drain → LED string cathode
      LED string anode → VCC ( current limiting is done by setting the max PWM)
  
    Licensed for personal, non-commercial use only.
    Commercial use requires written permission.
    
    Feel free to modify for your own projects!

    Version 0.1
    January 2026
*/


#include <avr/pgmspace.h>

#define LED_PIN     0
#define MODE_PIN    1
#define STEPS       128

// Basic settings
unsigned long STANDARD_CYCLE = 5000;    // ms
float STANDARD_PAUSE_FRAC = 0.1;        // 10% basic pause
uint8_t MIN_PWM = 8;                    // LED ember glow
uint8_t MAX_PWM = 200;                  // fixed max PWM (to prevent too much current to ledstring)

// Random mode settings
unsigned long RANDOM_CYCLE_MIN = 1000;  // 1 sec
unsigned long RANDOM_CYCLE_MAX = 20000; // 20 sec
float RANDOM_PAUSE_BASE = 0.1;          // 10% basic pause

// Gaussian Look Up Table 128 steps
const uint8_t gaussianLUT[STEPS] PROGMEM = {
   8,  9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
  24,26,28,30,32,34,36,38,40,43,46,49,52,56,60,64,
  68,73,78,83,88,94,100,106,112,119,126,133,140,148,
  156,164,172,180,188,196,204,212,220,228,236,244,252,255,
  252,244,236,228,220,212,204,196,188,180,172,164,156,148,
  140,133,126,119,112,106,100,94,88,83,78,73,68,64,60,56,
  52,49,46,43,40,38,36,34,32,30,28,26,24,23,22,21,
  20,19,18,17,16,15,14,13,12,11,10,9
};

// Variables
uint8_t stepIndex = 0;
unsigned long prevMs = 0;
unsigned long stepInterval = 0;
unsigned long pauseMs = 0;
unsigned long cycleMs = STANDARD_CYCLE;
bool inPause = false;

// Function to calculate stepInterval and pause
void calcPause(unsigned long cycleTime, float baseFrac, unsigned long* stepIntervalOut, unsigned long* pauseMsOut) {
  float minFrac = 0.0;
  float maxFrac = min(1.0, 2*baseFrac); // ±100% of base
  float pauseFraction = random((int)(minFrac*10000), (int)(maxFrac*10000)+1)/10000.0;

  *pauseMsOut = cycleTime * pauseFraction;
  *stepIntervalOut = cycleTime * (1.0 - pauseFraction) / STEPS;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MODE_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));

  // init standaard timing
  calcPause(STANDARD_CYCLE, STANDARD_PAUSE_FRAC, &stepInterval, &pauseMs);
}

void loop() {
  unsigned long now = millis();
  bool standardMode = !digitalRead(MODE_PIN); // LOW = standard, HIGH = random

  // Start new cycle at stepIndex==0 & not at pause
  if (stepIndex == 0 && !inPause) {
    if (standardMode) {
      cycleMs = STANDARD_CYCLE;
      calcPause(cycleMs, STANDARD_PAUSE_FRAC, &stepInterval, &pauseMs);
    } else {
      cycleMs = random(RANDOM_CYCLE_MIN, RANDOM_CYCLE_MAX + 1);
      calcPause(cycleMs, RANDOM_PAUSE_BASE, &stepInterval, &pauseMs);
    }
  }

  if (!inPause) {
    if (now - prevMs >= stepInterval) {
      prevMs = now;

      uint8_t pwm_val = pgm_read_byte(&gaussianLUT[stepIndex]);
      if (pwm_val < MIN_PWM) pwm_val = MIN_PWM;
      if (pwm_val > MAX_PWM) pwm_val = MAX_PWM; // vaste max PWM
      analogWrite(LED_PIN, pwm_val);

      stepIndex++;
      if (stepIndex >= STEPS) {
        stepIndex = 0;
        inPause = true;
        prevMs = now;
      }
    }
  } else {
    // pause: LED ember glow
    analogWrite(LED_PIN, MIN_PWM);
    if (now - prevMs >= pauseMs) {
      prevMs = now;
      inPause = false;
      stepIndex = 0; // nieuwe cyclus
    }
  }
}
