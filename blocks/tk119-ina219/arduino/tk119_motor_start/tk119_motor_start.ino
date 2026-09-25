// TK119 INA219: catch a motor starting, on an ESP32-S3.
//
// Wiring, TK119 header left to right (parts up, pins down):
//   GND -> ESP32 GND
//   3V3 -> ESP32 3V3
//   SCL -> GPIO9     (the third pin: the clock comes first)
//   SDA -> GPIO8
// Then the rail: the motor supply's + to POWER +, - to
// POWER -, and the motor or its driver across LOAD + and -.
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// No library needed. Serial Monitor at 115200.

#include <Wire.h>

const int SDA_PIN = 8, SCL_PIN = 9;
const uint8_t ADDR = 0x40;

// Config: 32 V range, gain /8 (3.2 A), shunt only,
// continuous. ONE_SAMPLE is a new reading every 532 us;
// AVERAGE_128 is one reading every 68 ms, the mean of 128.
const uint16_t ONE_SAMPLE = 0x399D;
const uint16_t AVERAGE_128 = 0x39FD;

void writeReg(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.write(value >> 8);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

int16_t readReg(uint8_t reg) {
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(ADDR, (uint8_t)2);
  uint8_t hi = Wire.read();
  uint8_t lo = Wire.read();
  return (int16_t)((hi << 8) | lo);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  writeReg(0x00, ONE_SAMPLE);  // try AVERAGE_128 to compare
  Serial.println("peak_mA  mean_mA  (each line is 200 ms)");
}

void loop() {
  float peak = 0, sum = 0;
  int n = 0;
  unsigned long t0 = millis();
  while (millis() - t0 < 200) {
    // Shunt register: 10 uV a count, which is 0.1 mA here.
    float ma = readReg(0x01) * 0.1;
    if (ma > peak) peak = ma;
    sum += ma;
    n++;
  }
  Serial.printf("%7.1f  %7.1f\n", peak, sum / n);
}
