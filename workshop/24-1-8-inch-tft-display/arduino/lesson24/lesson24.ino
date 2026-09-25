// 1.8" TFT (ST7735): SPI with CS/DC/RST, text and solid colors
#include <Arduino_GFX_Library.h>
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
// ST7735 1.8" 128x160 (adjust offset if image is shifted)
Arduino_GFX *gfx = new Arduino_ST7735(
  bus, TFT_RST, 0 /* rotation */, false /* IPS */,
  128 /* width */, 160 /* height */,
  0 /* col offset 1 */, 0 /* row offset 1 */,
  0 /* col offset 2 */, 0 /* row offset 2 */,
  false /* BGR */);

void setup() {
  gfx->begin();
  gfx->fillScreen(RGB565_BLACK);
  gfx->setTextColor(RGB565_WHITE);
  gfx->setTextSize(2);
  gfx->setCursor(10, 10);
  gfx->println("Hello!");   // Show once on power-up
}

void loop() {
  gfx->fillScreen(RGB565_BLACK);
  gfx->setCursor(10, 10);
  gfx->println("TFT Screen");
  delay(1000);
  gfx->fillScreen(RGB565_RED);    // Red 1 s
  delay(1000);
  gfx->fillScreen(RGB565_GREEN);  // Green 1 s
  delay(1000);
  gfx->fillScreen(RGB565_BLUE);   // Blue 1 s
  delay(1000);
}
