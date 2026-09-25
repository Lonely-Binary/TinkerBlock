// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int MIC_PIN = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Highest minus lowest over 50 ms: the swing, which grows with
  // loudness. The resting level cancels out of it.
  int lo = 32767;
  int hi = -1;
  unsigned long start = millis();
  while (millis() - start < 50) {
    int r = analogRead(MIC_PIN);
    if (r < lo) lo = r;
    if (r > hi) hi = r;
  }
  Serial.println(hi - lo);
}
