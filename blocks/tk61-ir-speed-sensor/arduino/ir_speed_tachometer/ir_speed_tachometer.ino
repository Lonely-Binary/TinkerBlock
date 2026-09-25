/*
  Infrared Speed Sensor - tachometer                    TK61 / /p/tk61

  Wiring, the same as the first read. Count from the square pad on
  the TinkerBlock board, parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              D2 on an Uno, GP15 on a Raspberry Pi Pico

  Mounting: a slotted disc on the shaft, its rim through the slot
  so each bar breaks the beam once as it passes.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int SENSOR_PIN = 4;

// Openings in your disc: one pulse each, every turn.
const int SLOTS = 20;

// Count for this long, then print.
const unsigned long WINDOW_MS = 1000;

// Ignore a second edge this soon after the last one.
const unsigned long LOCKOUT_US = 500;

// ESP32 cores want interrupt handlers in IRAM; others do not care.
#ifndef IRAM_ATTR
#define IRAM_ATTR
#endif

volatile unsigned long pulses = 0;
volatile unsigned long lastEdgeUs = 0;

// Runs on every rising edge: a bar has just entered the beam.
void IRAM_ATTR onEdge() {
  unsigned long at = micros();
  if (at - lastEdgeUs >= LOCKOUT_US) {
    pulses++;
    lastEdgeUs = at;
  }
}

unsigned long windowStart;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);   // the board drives SIGNAL itself
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), onEdge, RISING);
  windowStart = millis();
}

void loop() {
  if (millis() - windowStart < WINDOW_MS) return;

  // Take the count and start the next one, with the handler paused
  // so it cannot change pulses half way through the copy.
  noInterrupts();
  unsigned long n = pulses;
  pulses = 0;
  interrupts();

  unsigned long elapsed = millis() - windowStart;
  windowStart += elapsed;

  float perSecond = n * 1000.0 / elapsed;
  float rpm = perSecond * 60.0 / SLOTS;

  Serial.print(n);
  Serial.print(" pulses   ");
  Serial.print(rpm, 0);
  Serial.println(" rpm");
}
