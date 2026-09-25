/*
  Rotary Encoder - a state table                       TK06 / /p/tk06

  Wiring. Count from the square pad on the TinkerBlock board, knob
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (CLOCK, DATA and BTN all reach whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    BTN    -> D4 on an Uno, GPIO 27 on an ESP32, GPIO 6 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico
    CLOCK  -> D2, GPIO 25, GPIO 4, GP13   (the same four boards)
    DATA   -> D3, GPIO 26, GPIO 5, GP14

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// CLOCK, DATA and BTN, as GPIO numbers.
// Uno: 2, 3, 4. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
const int CLOCK_PIN = 4;
const int DATA_PIN = 5;
const int BUTTON_PIN = 6;       // not used in this sketch
const int STEPS_PER_CLICK = 4;  // on many EC11 parts; count yours

#ifndef IRAM_ATTR
#define IRAM_ATTR               // only the ESP32 cores need it
#endif

// +1 a step forward, -1 a step back, 0 no move or a missed state.
// Index: old state * 4 + new state. A state is CLOCK * 2 + DATA.
const int8_t TABLE[16] = {0, -1, 1, 0, 1, 0, 0, -1,
                          -1, 0, 0, 1, 0, 1, -1, 0};

volatile uint8_t state = 3;     // both HIGH: at rest
volatile long steps = 0;

// Runs on every change of either pin.
void IRAM_ATTR onEither() {
  uint8_t now = (digitalRead(CLOCK_PIN) << 1) | digitalRead(DATA_PIN);
  steps += TABLE[(state << 2) | now];
  state = now;
}

void setup() {
  Serial.begin(115200);
  pinMode(CLOCK_PIN, INPUT);    // the block has its own pull-ups
  pinMode(DATA_PIN, INPUT);
  state = (digitalRead(CLOCK_PIN) << 1) | digitalRead(DATA_PIN);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onEither, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DATA_PIN), onEither, CHANGE);
}

void loop() {
  static long shown = 0;

  noInterrupts();               // copy it in one piece
  long now = steps;
  interrupts();

  long clicks = now / STEPS_PER_CLICK;
  if (clicks != shown) {
    Serial.println(clicks);
    shown = clicks;
  }
}
