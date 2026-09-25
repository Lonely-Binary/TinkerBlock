#include <Wire.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Wire.begin();

  Serial.println("scanning");
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.print("found 0x");
      Serial.println(a, HEX);
    }
  }
  Serial.println("done");
}

void loop() {}
