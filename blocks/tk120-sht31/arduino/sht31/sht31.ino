/*
  SHT31 - temperature and humidity                      TK120 / /p/tk120

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
    Library Manager               "Adafruit SHT31 Library" by Adafruit,
                                  and accept "Adafruit BusIO" with it
    Serial Monitor                115200
*/

#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // 0x44 as shipped; 0x45 if the ADDR pads on the back are bridged.
  // begin() brings up Wire itself, on this board's default SDA and SCL.
  if (!sht.begin(0x44)) {
    Serial.println("no SHT31 at 0x44 - check GND, then SDA and SCL");
    while (true) delay(100);
  }
}

void loop() {
  float celsius, humidity;

  // One measurement, both numbers. readTemperature() and readHumidity()
  // would take two — this library caches nothing at all.
  if (sht.readBoth(&celsius, &humidity)) {
    Serial.print(celsius);  Serial.print(" C  ");
    Serial.print(humidity); Serial.println(" %");
  } else {
    Serial.println("read failed");   // a checksum did not match
  }

  delay(2000);
}
