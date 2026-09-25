// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUMPER_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(BUMPER_PIN, INPUT);   // the block has its own pull-down
}

void loop() {
  if (digitalRead(BUMPER_PIN) == HIGH) {   // HIGH is a hit
    Serial.println("hit");
  } else {
    Serial.println("clear");
  }
  delay(200);
}
