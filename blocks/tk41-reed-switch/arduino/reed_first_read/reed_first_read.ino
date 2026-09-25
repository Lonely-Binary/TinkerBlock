/*
  Reed Switch - first read                              TK41 / /p/tk41

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              Your board's logic voltage: a closed switch puts
              VCC straight onto the pin.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 25 on an ESP32,
              D2 on an Uno, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int REED_PIN = 4;

int last = -1;   // nothing read yet

void setup() {
  Serial.begin(115200);

  // INPUT, not INPUT_PULLUP: the board has its own 10k pull-down.
  pinMode(REED_PIN, INPUT);
}

void loop() {
  int level = digitalRead(REED_PIN);

  if (level != last) {
    if (level == HIGH) {
      Serial.println("magnet: switch closed");
    } else {
      Serial.println("no magnet: switch open");
    }
    last = level;
  }

  delay(10);
}
