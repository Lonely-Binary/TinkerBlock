/*
  DHT22 - first reading                                 TK39 / /p/tk39

  Wiring. Count from the square pad on the TinkerBlock board, sensor
  side up, header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico.
            DATA idles at VCC through the board's 10k pull-up,
            so VCC is your board's logic voltage.
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

// The pin DATA is wired to.
// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  4
#define DHT_TYPE DHT22   // the white cage. DHT11 is the blue one.

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // First, so the first read comes 2 s after power-up: the
  // datasheet wants 1 s of quiet, and the DHT22 more than 2 s
  // between readings.
  delay(2000);

  // One exchange on the wire. The second call is answered from
  // the library's copy of the same five bytes.
  float humidity = dht.readHumidity();
  float celsius  = dht.readTemperature();

  // A failed read is nan, not an error. Check every time.
  if (isnan(humidity) || isnan(celsius)) {
    Serial.println("read failed");
    return;
  }

  Serial.print(celsius);
  Serial.print(" C  ");
  Serial.print(humidity);
  Serial.println(" %");
}
