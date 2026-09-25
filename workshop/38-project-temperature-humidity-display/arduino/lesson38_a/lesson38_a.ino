// Option 1: DHT11 temp/humidity → serial
#include <DHT.h>
#define DHTPIN 2        // DHT11 DATA on D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();   // Init DHT
  Serial.println("Temp/humidity display started");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  delay(2000);   // Update every 2 s
}
