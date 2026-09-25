/*
  1.8-inch TFT Display - redraw only what changed       TK89 / /p/tk89

  Wiring. The eight pins top to bottom, screen facing you, header on
  the left (the front prints BL RST DC SCL MO CS 3V3 GND):

    BL   -> GPIO 41 on an ESP32-S3, GPIO 32 on an ESP32, GP2 on a
            Pico, D7 on an Uno. The backlight is off until BL is
            HIGH.
    RST  -> GPIO 42 on an ESP32-S3, GPIO 4 on an ESP32, GP3 on a
            Pico, D8 on an Uno
    DC   -> GPIO 2 on an ESP32-S3, GPIO 2 on an ESP32, GP4 on a
            Pico, D9 on an Uno
    SCL  -> the SPI clock: GPIO 12 on an ESP32-S3, GPIO 18 on an
            ESP32, GP18 on a Pico, D13 on an Uno
    MO   -> MOSI, SPI data out: GPIO 11 on an ESP32-S3, GPIO 23 on
            an ESP32, GP19 on a Pico, D11 on an Uno
    CS   -> GPIO 10 on an ESP32-S3, GPIO 15 on an ESP32, GP5 on a
            Pico, D10 on an Uno
    3V3  -> 3V3. Never 5V: the display is a 3.3 V part.
    GND  -> GND

  An Uno is a 5 V board: every line goes through a TK97 logic
  level converter, never straight to these pins.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      Adafruit ST7735 and ST7789 Library,
                                  and install its dependencies
                                  (Adafruit GFX, Adafruit BusIO)
*/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#if defined(ARDUINO_ARCH_AVR)              // Uno, through a TK97
const int TFT_CS = 10, TFT_DC = 9, TFT_RST = 8, TFT_BL = 7;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)   // ESP32-S3
const int TFT_CS = 10, TFT_DC = 2, TFT_RST = 42, TFT_BL = 41;
#elif defined(ARDUINO_ARCH_ESP32)          // ESP32
const int TFT_CS = 15, TFT_DC = 2, TFT_RST = 4, TFT_BL = 32;
#else                                      // Raspberry Pi Pico
const int TFT_CS = 5, TFT_DC = 4, TFT_RST = 3, TFT_BL = 2;
#endif

// true: clear the whole screen and draw everything, every frame.
// Try it once, watch the flicker, and read the time it prints.
const bool CLEAR_EVERY_FRAME = false;

const uint16_t BG = ST77XX_BLACK;
const uint16_t INK = ST77XX_WHITE;
const uint16_t BAR = ST77XX_GREEN;
const int BAR_X = 10, BAR_Y = 90, BAR_W = 140, BAR_H = 14;

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
int barWas = 0;   // the bar's width on the screen now

// The parts that never change: drawn once.
void drawFrame() {
  tft.fillScreen(BG);
  tft.setTextColor(INK);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("TK89  uptime");
  tft.setCursor(124, 57);
  tft.print("s");
  tft.drawRect(BAR_X - 1, BAR_Y - 1, BAR_W + 2, BAR_H + 2, INK);
  barWas = 0;
}

// The parts that change. The text is drawn with a background
// colour, so each character cell paints over the old one: no
// clearing, nothing blank for a moment, nothing to flicker.
void drawValue(unsigned long tenths) {
  char text[12];
  snprintf(text, sizeof text, "%4lu.%lu", tenths / 10, tenths % 10);
  tft.setTextColor(INK, BG);
  tft.setTextSize(3);
  tft.setCursor(10, 40);
  tft.print(text);

  // The bar: paint only the strip between the old and new ends.
  int bar = (int)((tenths % 100) * BAR_W / 100);
  if (bar > barWas) {
    tft.fillRect(BAR_X + barWas, BAR_Y, bar - barWas, BAR_H, BAR);
  } else if (bar < barWas) {
    tft.fillRect(BAR_X + bar, BAR_Y, barWas - bar, BAR_H, BG);
  }
  barWas = bar;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);   // the backlight is off until then

  tft.initR(INITR_BLACKTAB);    // see first light if colours are off
  // tft.invertDisplay(true);   // if first light needed it
  tft.setRotation(1);           // landscape, 160 x 128
  drawFrame();
}

void loop() {
  static unsigned long lastPrint = 0;
  unsigned long tenths = millis() / 100;

  unsigned long t0 = micros();
  if (CLEAR_EVERY_FRAME) drawFrame();
  drawValue(tenths);
  unsigned long took = micros() - t0;

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    Serial.print(CLEAR_EVERY_FRAME ? "everything: " : "changes: ");
    Serial.print(took);
    Serial.println(" us");
  }
  delay(100);
}
