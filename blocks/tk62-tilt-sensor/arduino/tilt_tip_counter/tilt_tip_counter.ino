/*
  Tilt Sensor - counting tips                           TK62 / /p/tk62

  Wiring, the same as the first read. Count from the square pad on
  the TinkerBlock board, parts up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              D2 on an Uno, GP15 on a Raspberry Pi Pico

  Mounting: lay the block along the thing that tips, a see-saw or a
  plank, with the board's left and right edges toward its two ends.
  Each tip from one end down to the other is one count.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int TILT_PIN = 4;

// A change counts once it has held this long: longer than the
// balls rattle, shorter than a real tip.
const unsigned long STABLE_MS = 50;

int raw;                    // the last reading
int steady;                 // the last reading that held
unsigned long rawSince;     // when raw last changed
unsigned long tips = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TILT_PIN, INPUT);   // the board has its own pull-down

  raw = digitalRead(TILT_PIN);
  steady = raw;
  rawSince = millis();
  Serial.println(steady == HIGH ? "left end down" : "right end down");
}

void loop() {
  int level = digitalRead(TILT_PIN);
  if (level != raw) {         // a rattle, or a real tip starting
    raw = level;
    rawSince = millis();
  }

  // Believe a change only once it has held for STABLE_MS.
  if (raw != steady && millis() - rawSince >= STABLE_MS) {
    steady = raw;
    tips++;
    Serial.print("tip ");
    Serial.print(tips);
    Serial.println(steady == HIGH ? ": left end down"
                                  : ": right end down");
  }
}
