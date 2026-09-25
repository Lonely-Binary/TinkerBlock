/*
  Push Button - first read                               TK04 / /p/tk04

  Wiring. Count from the square pad on the TinkerBlock board, button
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (pressed, SIGNAL gives your pin whatever VCC is)
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
const int BUTTON_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  int level = digitalRead(BUTTON_PIN);

  if (level == HIGH) {
    Serial.println("HIGH  pressed");
  } else {
    Serial.println("LOW   released");
  }
  delay(200);                   // five reads a second, to keep it readable
}
