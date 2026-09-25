// The pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int POT_PIN = 4;

// What analogRead returns at full scale on your board.
// Uno: 1023. ESP32, ESP32-S3: 4095. Pico: 1023.
const int ADC_MAX = 4095;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int reading = analogRead(POT_PIN);
  int percent = map(reading, 0, ADC_MAX, 0, 100);

  Serial.print(reading);
  Serial.print("   ");
  Serial.print(percent);
  Serial.println(" %");
  delay(200);
}
