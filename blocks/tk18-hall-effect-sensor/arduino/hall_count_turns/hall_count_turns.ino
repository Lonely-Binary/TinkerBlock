/*
  Hall Effect Sensor - counting turns                    TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (with no magnet, SIGNAL sits at whatever VCC is)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  One magnet on the wheel, a flat face passing the small chip at
  the top left of the block.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int HALL_PIN = 4;

bool lastMagnet = false;        // was the magnet there last time round?
unsigned long turns = 0;
unsigned long lastPass = 0;     // millis() at the last pass

void setup() {
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT);     // the chip drives SIGNAL both ways
}

void loop() {
  bool magnet = digitalRead(HALL_PIN) == LOW;   // active low

  if (magnet && !lastMagnet) {  // the magnet has just arrived
    unsigned long now = millis();
    turns++;
    Serial.print("turns: ");
    Serial.print(turns);
    if (lastPass != 0) {
      Serial.print("   rpm: ");
      Serial.print(60000.0 / (now - lastPass), 0);
    }
    Serial.println();
    lastPass = now;
  }
  lastMagnet = magnet;
}
