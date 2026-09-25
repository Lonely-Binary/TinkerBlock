/*
  Knock Sensor - a secret knock                          TK28 / /p/tk28

  Wiring, the TK28. Count from the square pad, switch at the top,
  header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (during a knock, SIGNAL gives your pin whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP14 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int KNOCK_PIN = 4;
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;

const unsigned long HOLD_OFF_MS = 100;  // longer than the spring rings
const unsigned long END_MS = 1500;      // this much silence ends a try
const unsigned long OPEN_MS = 3000;     // how long the LED stays on
const float TOLERANCE = 0.1;            // of the longest gap

// Shave and a haircut, two bits: the gaps between 7 knocks, in beats.
const int SECRET[] = {2, 1, 1, 2, 4, 2};
const int GAPS = 6;
const int MAX_KNOCKS = 10;

#ifndef IRAM_ATTR
#define IRAM_ATTR               // only the ESP32 cores need it
#endif

volatile unsigned long knockAt[MAX_KNOCKS];
volatile int knocks = 0;
volatile unsigned long lastKnock = 0;

// Runs the instant SIGNAL goes HIGH. Short, and no printing.
void IRAM_ATTR onKnock() {
  unsigned long now = millis();
  if (now - lastKnock < HOLD_OFF_MS) return;   // still ringing
  lastKnock = now;
  if (knocks < MAX_KNOCKS) knockAt[knocks] = now;
  knocks++;
}

bool matches(int n) {
  if (n != GAPS + 1) return false;            // wrong count
  unsigned long longest = 0;
  for (int i = 0; i < GAPS; i++) {
    unsigned long gap = knockAt[i + 1] - knockAt[i];
    if (gap > longest) longest = gap;
  }
  for (int i = 0; i < GAPS; i++) {
    float heard = (knockAt[i + 1] - knockAt[i]) / (float)longest;
    float wanted = SECRET[i] / 4.0;           // 4: SECRET's longest
    if (fabs(heard - wanted) > TOLERANCE) return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(KNOCK_PIN, INPUT);    // the block has its own pull-down
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(KNOCK_PIN), onKnock, RISING);
}

void loop() {
  noInterrupts();               // copy them in one piece
  int n = knocks;
  unsigned long last = lastKnock;
  interrupts();

  if (n > 0 && millis() - last > END_MS) {     // the knocking stopped
    bool open = matches(n);
    Serial.print(n);
    Serial.println(open ? " knocks: open" : " knocks: wrong");
    if (open) {
      digitalWrite(LED_PIN, HIGH);
      delay(OPEN_MS);
      digitalWrite(LED_PIN, LOW);
    }
    noInterrupts();
    knocks = 0;                 // ready for the next try
    interrupts();
  }
}
