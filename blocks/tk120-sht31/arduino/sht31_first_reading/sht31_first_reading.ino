/*
  SHT31 - first reading                                 TK120 / /p/tk120

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND -> GND
    VCC -> 3V3      (5V on an Arduino Uno. Never 5V beside a 3.3 V board:
                     the board's pull-ups put VCC on both signal lines.)
    SDA -> GPIO 8   (Uno A4, ESP32 GPIO 21, ESP32-S3 GPIO 8, Pico GP4)
    SCL -> GPIO 9   (Uno A5, ESP32 GPIO 22, ESP32-S3 GPIO 9, Pico GP5)

  Arduino IDE
    Tools > Board                 ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled
    Library Manager               "Adafruit SHT31 Library" by Adafruit,
                                  and accept "Adafruit BusIO" with it
*/

#include <Adafruit_SHT31.h>

// 0x44 is what a board answers to straight out of the bag, because R3
// holds the sensor's ADDR pin at ground. Bridge the ADDR pads on the
// back and it becomes 0x45.
#define SHT31_ADDR 0x44

Adafruit_SHT31 sht;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);        // native-USB boards: wait for the monitor

  // begin() brings up Wire itself, on whatever this board's default SDA
  // and SCL pins are. For other pins, call Wire.begin(sda, scl) first.
  if (!sht.begin(SHT31_ADDR)) {
    Serial.println("no SHT31 at 0x44 - check GND first, then SDA and SCL");
    while (true) delay(100);
  }

  Serial.println("SHT31 found");
}

void loop() {
  float celsius, humidity;

  // One command, one wait, one set of six bytes, both numbers.
  if (sht.readBoth(&celsius, &humidity)) {
    Serial.print(celsius);
    Serial.print(" C  ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    // A checksum that did not match, or nothing on the bus at all. One
    // failure now and then is normal; every time is the wiring.
    Serial.println("read failed");
  }

  delay(2000);
}
