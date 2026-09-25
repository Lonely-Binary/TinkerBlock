/*
  1.8-inch TFT Display - first light                    TK89 / /p/tk89

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

// CS, DC, RST and BL are any output pins. SCL and MO are the
// board's own SPI pins, which SPI picks for itself.
#if defined(ARDUINO_ARCH_AVR)              // Uno, through a TK97
const int TFT_CS = 10, TFT_DC = 9, TFT_RST = 8, TFT_BL = 7;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)   // ESP32-S3
const int TFT_CS = 10, TFT_DC = 2, TFT_RST = 42, TFT_BL = 41;
#elif defined(ARDUINO_ARCH_ESP32)          // ESP32
const int TFT_CS = 15, TFT_DC = 2, TFT_RST = 4, TFT_BL = 32;
#else                                      // Raspberry Pi Pico
const int TFT_CS = 5, TFT_DC = 4, TFT_RST = 3, TFT_BL = 2;
#endif

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

// Fill the screen and print the colour's name on it, so a
// swapped colour shows as a name on the wrong colour.
void panel(uint16_t colour, const char *name, uint16_t ink) {
  tft.fillScreen(colour);
  tft.setTextColor(ink);
  tft.setTextSize(2);
  tft.setCursor(8, 8);
  tft.print(name);
  Serial.print("now showing: ");
  Serial.println(name);
  delay(1500);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // The backlight first. Q2 on the board keeps it off until BL
  // is HIGH, so a dark screen here is BL, not the picture.
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // The panel's type. If the colours come out wrong or there is
  // a band of noise at one edge, try INITR_GREENTAB or
  // INITR_REDTAB here. The handbook's last chapter has the table.
  tft.initR(INITR_BLACKTAB);
  // If BLACK shows as white, the panel wants its colours inverted.
  // The owner's video of an older sketch suggests this one does:
  // take the // off the next line if so.
  // tft.invertDisplay(true);

  tft.setRotation(1);   // 1 or 3: landscape. 0 or 2: portrait.
  Serial.println("TK89 first light: 160 x 128, landscape");
}

void loop() {
  panel(ST77XX_RED, "RED", ST77XX_WHITE);
  panel(ST77XX_GREEN, "GREEN", ST77XX_BLACK);
  panel(ST77XX_BLUE, "BLUE", ST77XX_WHITE);
  panel(ST77XX_WHITE, "WHITE", ST77XX_BLACK);
  panel(ST77XX_BLACK, "BLACK", ST77XX_WHITE);

  tft.setTextSize(1);
  tft.setCursor(8, 40);
  tft.print(tft.width());
  tft.print(" x ");
  tft.print(tft.height());
  tft.print(" pixels");
  tft.drawRect(0, 0, tft.width(), tft.height(), ST77XX_YELLOW);
  delay(3000);
}
