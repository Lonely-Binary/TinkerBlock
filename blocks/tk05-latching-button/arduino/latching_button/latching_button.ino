// The GPIO number SIGNAL is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int SWITCH_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);   // the board's pull-down holds it LOW
}

void loop() {
  if (digitalRead(SWITCH_PIN) == HIGH) {
    Serial.println("ON");       // latched: SIGNAL is VCC
  } else {
    Serial.println("OFF");
  }
  delay(500);
}
