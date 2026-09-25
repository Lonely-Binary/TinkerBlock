/*
  DHT22 - a line every ten seconds                      TK39 / /p/tk39

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC   -> nothing   (unconnected on the board)
    DATA -> D2 on an Uno, GPIO 18 on an ESP32,
            GPIO 4 on an ESP32-S3, GP2 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      DHT sensor library, by Adafruit.
                                  Say yes to Adafruit Unified Sensor.
*/

#include <DHT.h>

// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// Well clear of the library's 2000 ms. Nothing in a room changes
// faster than this, and a day is 8640 lines.
const unsigned long EVERY_MS = 10000;
unsigned long last = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  last = millis();          // first line EVERY_MS after power-up
  Serial.println("s,celsius,humidity");
}

void loop() {
  // Measured from the last start, not the last finish, so the time
  // spent reading and printing does not push the next line later.
  if (millis() - last < EVERY_MS) return;
  last += EVERY_MS;

  float humidity = dht.readHumidity();
  float celsius  = dht.readTemperature();

  Serial.print(last / 1000);
  Serial.print(",");
  if (isnan(humidity) || isnan(celsius)) {
    Serial.println(",");    // a gap you can see in the log
    return;
  }
  Serial.print(celsius, 1);
  Serial.print(",");
  Serial.println(humidity, 1);
}
