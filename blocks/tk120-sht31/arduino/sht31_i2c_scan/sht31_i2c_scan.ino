/*
  SHT31 - I2C scan                                      TK120 / /p/tk120

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND -> GND
    VCC -> 3V3      (5V on an Arduino Uno. Never 5V beside a 3.3 V
                     board: the pull-ups put VCC on SDA and SCL.)
    SDA -> A4, GPIO 21, GPIO 8 or GP4
    SCL -> A5, GPIO 22, GPIO 9 or GP5

  Uno, ESP32, ESP32-S3, Pico, in that order: each board's default
  I2C pins, so nothing in the sketch names them.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
    Serial Monitor                115200
*/

// Lists every address that answers. A TK120 straight out of the bag is
// 0x44; with its ADDR pads bridged, 0x45.
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
