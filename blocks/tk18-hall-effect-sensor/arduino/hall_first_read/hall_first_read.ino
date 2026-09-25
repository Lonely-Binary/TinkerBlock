/*
  Hall Effect Sensor - first read                        TK18 / /p/tk18

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (with no magnet, SIGNAL sits at whatever VCC is)
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
const int HALL_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT);     // the chip drives SIGNAL both ways
}

void loop() {
  int level = digitalRead(HALL_PIN);

  if (level == LOW) {           // active low: LOW is a magnet
    Serial.println("LOW   magnet");
  } else {
    Serial.println("HIGH  no magnet");
  }
  delay(200);                   // five reads a second, to keep it readable
}
