// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int HALL_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(HALL_PIN, INPUT);   // the chip drives SIGNAL both ways
}

void loop() {
  if (digitalRead(HALL_PIN) == LOW) {   // active low: LOW is a magnet
    Serial.println("magnet");
  } else {
    Serial.println("no magnet");
  }
  delay(200);
}
