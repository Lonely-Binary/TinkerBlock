/*
  DHT11 - the first reading                            TK38 / /p/tk38

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  up, header at the bottom:

    GND  -> GND
    VCC  -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno
    NC   -> nothing   (connected to nothing on the board)
    DATA -> D2 on an Uno, GPIO 18 on an ESP32, GPIO 4 on an
            ESP32-S3, GP2 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Library Manager: "DHT sensor library" by Adafruit. Say yes when
    it offers Adafruit Unified Sensor: it will not compile without.
*/

#include <DHT.h>

// The pin DATA is wired to.
// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  2
#define DHT_TYPE DHT11   // the TK38 is a DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(1000);   // the datasheet: nothing for 1 s after power-up
}

void loop() {
  // Humidity first, then temperature. Both come out of one
  // reading: the library keeps the bytes for two seconds.
  float humidity = dht.readHumidity();
  float celsius  = dht.readTemperature();

  // A failed read returns nan, never an error. Check every time.
  if (isnan(humidity) || isnan(celsius)) {
    Serial.println("read failed");
  } else {
    Serial.print(celsius, 0);
    Serial.print(" C  ");
    Serial.print(humidity, 0);
    Serial.println(" %");
  }

  delay(2000);   // the library answers from memory inside 2 s
}
