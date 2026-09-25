/*
  EEPROM Memory - text across a page edge               TK31 / /p/tk31

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND -> GND
    VCC -> your board's logic supply: 5V on an Uno, 3V3 on an
           ESP32 or ESP32-S3, 3V3(OUT) on a Pico. Never 5V beside
           a 3.3 V board: the pull-ups would put 5 V on its pins.
    SDA -> A4 on an Uno, GPIO 21 on an ESP32, GPIO 8 on an
           ESP32-S3, GP4 on a Raspberry Pi Pico
    SCL -> A5 on an Uno, GPIO 22 on an ESP32, GPIO 9 on an
           ESP32-S3, GP5 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library to install: Wire comes with every board.
*/

#include <Wire.h>

const int EEPROM_ADDR = 0x50;
const uint16_t PAGE = 64;      // the chip's page
const uint16_t PIECE = 16;     // fits every board's Wire buffer
const uint16_t START = 50;     // crosses the edge at 64 on purpose
const char TEXT[] = "Written across a page edge, read back whole.";

void waitReady() {
  unsigned long t0 = millis();
  do {
    Wire.beginTransmission(EEPROM_ADDR);
  } while (Wire.endTransmission() != 0 && millis() - t0 < 20);
}

void writeBlock(uint16_t at, const uint8_t *data, uint16_t n) {
  while (n > 0) {
    uint16_t room = PAGE - at % PAGE;   // bytes left in this page
    uint16_t len = min(n, min(room, PIECE));
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((uint8_t)(at >> 8));
    Wire.write((uint8_t)(at & 0xFF));
    Wire.write(data, len);
    Wire.endTransmission();
    waitReady();                        // one wait per piece
    at += len;
    data += len;
    n -= len;
  }
}

void readBlock(uint16_t at, uint8_t *out, uint16_t n) {
  while (n > 0) {
    uint16_t len = min(n, PIECE);
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((uint8_t)(at >> 8));
    Wire.write((uint8_t)(at & 0xFF));
    Wire.endTransmission(false);
    Wire.requestFrom(EEPROM_ADDR, (int)len);
    for (uint16_t i = 0; i < len; i++) {
      out[i] = Wire.available() ? Wire.read() : '?';
    }
    at += len;
    out += len;
    n -= len;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  writeBlock(START, (const uint8_t *)TEXT, sizeof(TEXT));

  char back[sizeof(TEXT)];
  readBlock(START, (uint8_t *)back, sizeof(back));
  back[sizeof(back) - 1] = '\0';
  Serial.print("wrote: ");
  Serial.println(TEXT);
  Serial.print("read:  ");
  Serial.println(back);
}

void loop() {
}
