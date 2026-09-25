// TK37 Passive Buzzer: the first beep, on an ESP32-S3.
//
// Wiring, TK37 header left to right (parts up, pins down):
//   GND    -> ESP32 GND
//   VCC    -> ESP32 3V3   (5V works too, and is louder)
//   NC     -> nothing
//   SIGNAL -> GPIO4
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// No library needed. Serial Monitor at 115200.

// The pin SIGNAL is wired to. Uno: 9. ESP32: 4. Pico: 15.
const int BUZZER_PIN = 4;

void setup() {
  Serial.begin(115200);
}

void loop() {
  tone(BUZZER_PIN, 2700, 200);  // 2.7 kHz for 200 ms, then LOW
  Serial.println("beep at 2700 Hz");
  delay(1000);                  // tone() returns at once

  tone(BUZZER_PIN, 440, 200);   // A4: lower, and quieter
  Serial.println("beep at 440 Hz");
  delay(1000);
}
