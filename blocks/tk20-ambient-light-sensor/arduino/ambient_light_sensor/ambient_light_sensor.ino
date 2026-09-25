// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int LIGHT_PIN = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // The mean of everything read in 50 ms: whole cycles of a
  // mains lamp's 100 or 120 Hz ripple, so the ripple cancels.
  long sum = 0;
  long n = 0;
  unsigned long start = millis();
  while (millis() - start < 50) {
    sum += analogRead(LIGHT_PIN);   // more light, higher
    n++;
  }
  Serial.println(sum / n);
  delay(200);
}
