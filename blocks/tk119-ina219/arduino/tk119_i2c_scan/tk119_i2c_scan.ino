#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin(8, 9);  // ESP32-S3: SDA 8, SCL 9
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) Serial.printf("found 0x%02X\n", a);
  }
}

void loop() {}
