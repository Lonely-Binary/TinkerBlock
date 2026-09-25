/*
  Rotary Encoder - counted by an interrupt             TK06 / /p/tk06

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

#ifndef IRAM_ATTR
#define IRAM_ATTR               // only the ESP32 cores need it
#endif

volatile long count = 0;        // changed inside the handler

// Runs the instant CLOCK falls. Short, and no printing.
void IRAM_ATTR onClockFall() {
  if (digitalRead(DATA_PIN) == HIGH) {
    count++;                    // CLOCK fell first
  } else {
    count--;                    // DATA fell first
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(CLOCK_PIN, INPUT);    // the block has its own pull-ups
  pinMode(DATA_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), onClockFall,
                  FALLING);
}

void loop() {
  static long shown = 0;

  noInterrupts();               // copy it in one piece
  long now = count;
  interrupts();

  if (now != shown) {
    Serial.println(now);
    shown = now;
  }
  delay(50);                    // busy elsewhere: the count survives
}
