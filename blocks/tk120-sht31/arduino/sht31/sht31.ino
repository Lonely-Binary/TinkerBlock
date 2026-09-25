#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // 0x44 as shipped; 0x45 if the ADDR pads on the back are bridged.
  // begin() brings up Wire itself, on this board's default SDA and SCL.
  if (!sht.begin(0x44)) {
    Serial.println("no SHT31 at 0x44 - check GND first, then SDA and SCL");
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
