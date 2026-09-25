#include <DHT.h>

// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  2
#define DHT_TYPE DHT11   // the TK38 is a DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(1000);   // nothing for 1 s after power-up
}

void loop() {
  float humidity = dht.readHumidity();
  float celsius  = dht.readTemperature();

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
