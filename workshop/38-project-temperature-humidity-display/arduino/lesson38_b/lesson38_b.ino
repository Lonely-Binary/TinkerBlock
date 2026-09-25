// Option 2: DHT11 temperature → 4-digit display
#include <DHT.h>
#include <TM1637Display.h>
#define DHTPIN 2
#define DHTTYPE DHT11
#define CLK_PIN 3   // TM1637 CLK
#define DIO_PIN 4   // TM1637 DIO

DHT dht(DHTPIN, DHTTYPE);
TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  dht.begin();
  display.setBrightness(7);   // Display brightness 0-7
}

void loop() {
  float temperature = dht.readTemperature();
  display.showNumberDec((int)temperature);   // Integer part only
  delay(2000);
}
