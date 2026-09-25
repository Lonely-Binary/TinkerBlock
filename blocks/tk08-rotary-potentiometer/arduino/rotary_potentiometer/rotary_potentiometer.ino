// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int POT_PIN = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int reading = analogRead(POT_PIN);   // 0 to 1023 on an Uno
  Serial.println(reading);
  delay(200);
}
