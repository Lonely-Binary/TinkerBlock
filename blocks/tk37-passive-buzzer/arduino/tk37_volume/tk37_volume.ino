// TK37 Passive Buzzer: volume from the duty cycle, ESP32-S3.
//
// Wiring, TK37 header left to right (parts up, pins down):
//   GND    -> ESP32 GND
//   VCC    -> ESP32 3V3   (5V works too, and is louder)
//   NC     -> nothing
//   SIGNAL -> GPIO4
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// esp32 core 3.x (ledcAttach). Serial Monitor at 115200.

const int BUZZER_PIN = 4;
const int HZ = 2700;  // the buzzer's resonance: its loudest pitch

// Duty, of 255. 128 is 50 %, the loudest; 0 is silent. Never
// 255: that is SIGNAL held HIGH, silent, with the coil on.
const int LEVELS[] = {128, 64, 26, 13, 0};

void setup() {
  Serial.begin(115200);
  ledcAttach(BUZZER_PIN, HZ, 8);  // 8 bits: duty 0 to 255
}

void loop() {
  for (int duty : LEVELS) {
    Serial.printf("duty %d of 255\n", duty);
    ledcWrite(BUZZER_PIN, duty);
    delay(700);
    ledcWrite(BUZZER_PIN, 0);     // 0: LOW, quiet, coil off
    delay(300);
  }
  delay(1500);
}
