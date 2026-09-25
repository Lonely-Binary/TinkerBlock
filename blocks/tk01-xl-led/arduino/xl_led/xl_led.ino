// The GPIO number SIGNAL is wired to. Uno: 9. ESP32, ESP32-S3: 4. Pico: 15.
const int LED_PIN = 4;

void setup() {
  pinMode(LED_PIN, OUTPUT);     // without this, HIGH is only a weak pull-up
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // the pin supplies the LED's current
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
