/*
  Latching button - acting on a change                  TK05 / /p/tk05

  Wiring. Count from the square pad on the TinkerBlock board, switch
  side up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (never 5V on a 3.3 V board: SIGNAL is VCC when on)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int SWITCH_PIN = 4;
const unsigned long SETTLE_MS = 20;   // a new level must hold this long

int settled;                  // the level the sketch believes
int lastRead;                 // the pin at the last read, bounces and all
unsigned long changedAt = 0;  // when lastRead last changed

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);   // the board's pull-down holds it LOW
  settled = digitalRead(SWITCH_PIN);
  lastRead = settled;
  Serial.println(settled == HIGH ? "Starts ON" : "Starts OFF");
}

void loop() {
  int now = digitalRead(SWITCH_PIN);

  if (now != lastRead) {        // it moved, or it bounced: restart clock
    lastRead = now;
    changedAt = millis();
  }

  // A change is a new level that has held for SETTLE_MS.
  if (now != settled && millis() - changedAt >= SETTLE_MS) {
    settled = now;
    Serial.println(settled == HIGH ? "Turned ON" : "Turned OFF");
  }

  // Anything else goes here. Nothing above waits.
}
