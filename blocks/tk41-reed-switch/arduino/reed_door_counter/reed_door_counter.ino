/*
  Reed Switch - a door that counts                      TK41 / /p/tk41

  Wiring, the same as the first read. Count from the square pad on
  the TinkerBlock board, parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              D2 on an Uno, GP15 on a Raspberry Pi Pico

  Mounting: the block on the frame, the magnet on the door, so
  the switch is closed (HIGH) while the door is shut.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int REED_PIN = 4;

// A change counts once it has held this long.
const unsigned long DEBOUNCE_MS = 20;

int raw;                    // the last reading
int steady;                 // the last reading that held
unsigned long rawSince;     // when raw last changed
unsigned long openedAt;     // when the door last opened
unsigned long openings = 0;

void setup() {
  Serial.begin(115200);
  pinMode(REED_PIN, INPUT);   // the board has its own pull-down

  raw = digitalRead(REED_PIN);
  steady = raw;
  rawSince = millis();
  openedAt = millis();
  Serial.println(steady == HIGH ? "door shut" : "door open");
}

void loop() {
  int level = digitalRead(REED_PIN);
  if (level != raw) {         // a bounce, or a real change starting
    raw = level;
    rawSince = millis();
  }

  // Believe a change only once it has held for DEBOUNCE_MS.
  if (raw != steady && millis() - rawSince >= DEBOUNCE_MS) {
    steady = raw;
    if (steady == LOW) {      // magnet gone: the door opened
      openings++;
      openedAt = millis();
      Serial.print("door opened, time ");
      Serial.println(openings);
    } else {                  // magnet back: the door shut
      Serial.print("door shut after ");
      Serial.print((millis() - openedAt) / 1000.0, 1);
      Serial.println(" s");
    }
  }
}
