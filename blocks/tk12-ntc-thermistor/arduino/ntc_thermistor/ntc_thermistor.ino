#include <math.h>

// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SENSOR_PIN = A0;

void setup() {
  Serial.begin(115200);
}

void loop() {
#if defined(ARDUINO_ARCH_ESP32)
  float mv = analogReadMilliVolts(SENSOR_PIN);     // own reference
  float r = 10000.0 * mv / (3300.0 - mv);          // VCC taken as 3.3 V
#else
  float n = analogRead(SENSOR_PIN);                // a fraction of VCC
  float r = 10000.0 * n / (1023.0 - n);
#endif
  float c = 1.0 / (1.0 / 298.15 + log(r / 10000.0) / 3950.0) - 273.15;
  Serial.print(c, 1);
  Serial.println(" C");
  delay(1000);
}
