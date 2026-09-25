/*
  LM75 Temperature Sensor - first reading           TK42 / /p/tk42

  Wiring. Count from the square pad on the TinkerBlock board,
  parts up, header at the bottom:

    GND -> GND
    VCC -> your board's logic supply: 5V on an Uno, 3V3 on an
           ESP32 or ESP32-S3, 3V3(OUT) on a Pico. Never 5V
           beside a 3.3 V board.
    SDA -> A4 on an Uno, GPIO 21 on an ESP32, GPIO 8 on an
           ESP32-S3, GP4 on a Raspberry Pi Pico
    SCL -> A5 on an Uno, GPIO 22 on an ESP32, GPIO 9 on an
           ESP32-S3, GP5 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board            your board, e.g. Arduino Uno
    Tools > Port             the one that appears when you plug in
    Tools > USB CDC On Boot  Enabled   (ESP32-S3 only)
    No library to install: Wire comes with every board.
    Serial Monitor at 115200.
*/

#include <Wire.h>

const int LM75_ADDR = 0x48;  // fixed: A0 to A2 are tied to GND

// True, and the temperature in c, if the chip answered.
bool readCelsius(float &c) {
  Wire.beginTransmission(LM75_ADDR);
  Wire.write(0x00);                    // register 0: temperature
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(LM75_ADDR, 2) != 2) return false;
  uint8_t hi = Wire.read();            // whole degrees
  uint8_t lo = Wire.read();            // the fraction
  int16_t raw = (int16_t)((hi << 8) | lo);
  c = raw / 256.0;                     // right below zero too
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);                         // time for the monitor
  Wire.begin();                        // the default I2C pins
  float c;
  readCelsius(c);                      // the first one is thrown away
}

void loop() {
  float c;
  if (readCelsius(c)) {
    Serial.print(c, 3);
    Serial.println(" C");
  } else {
    Serial.println("No LM75 at 0x48: check SDA and SCL.");
  }
  delay(500);
}
