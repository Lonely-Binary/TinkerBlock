// TFT shows DHT11 temp/humidity; refresh every 2 s
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <DHT.h>
#define CS_PIN 10   // TFT SPI CS
#define RST_PIN 8
#define DC_PIN 9
#define DHTPIN 2    // DHT11 DATA
#define DHTTYPE DHT11

Adafruit_ST7735 tft = Adafruit_ST7735(CS_PIN, DC_PIN, RST_PIN);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  tft.fillScreen(ST7735_BLACK);   // Clear before redraw to avoid ghosting
  tft.setCursor(10, 10);
  tft.print("Temp: ");
  tft.print(temperature);
  tft.println("C");
  tft.setCursor(10, 40);
  tft.print("Humi: ");
  tft.print(humidity);
  tft.println("%");
  delay(2000);   // Update every 2 s
}
