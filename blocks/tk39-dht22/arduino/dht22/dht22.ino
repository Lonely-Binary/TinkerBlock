#include <DHT.h>

// Uno: 2. ESP32: 18. ESP32-S3: 4. Pico: 2.
#define DHT_PIN  4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);   // more than 2 s between readings, and 1 s after power-up

  float humidity = dht.readHumidity();
  float celsius  = dht.readTemperature();

  if (isnan(humidity) || isnan(celsius)) {
    Serial.println("read failed");   // nothing answered
    return;
  }
  Serial.print(celsius, 1);  Serial.print(" C  ");
  Serial.print(humidity, 1); Serial.println(" %");
}
