// The pin SIG is wired to. Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SIG_PIN = A0;

// Uno: 5.0 and 1023. Pico: 3.3 and 1023. The ESP32s do not use these.
const float VREF = 5.0;
const float ADC_MAX = 1023.0;

float sigVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(SIG_PIN) / 1000.0;  // calibrated
#else
  return analogRead(SIG_PIN) * VREF / ADC_MAX;
#endif
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print(sigVolts() * 5.0, 2);  // the terminal is 5 x SIG
  Serial.println(" V");
  delay(500);
}
