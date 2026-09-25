/*
  EEPROM Memory - save a setting, only when it changes  TK31 / /p/tk31

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
    Serial monitor: 115200 baud, line ending Newline.
    No library to install: Wire comes with every board.
*/

#include <Wire.h>

const int EEPROM_ADDR = 0x50;
const uint16_t SETTING_AT = 16;   // any address; move it if it wears

unsigned long writes = 0;         // how many real writes, this run

void waitReady() {
  unsigned long t0 = millis();
  do {
    Wire.beginTransmission(EEPROM_ADDR);
  } while (Wire.endTransmission() != 0 && millis() - t0 < 20);
}

int readByte(uint16_t at) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));
  Wire.write((uint8_t)(at & 0xFF));
  if (Wire.endTransmission(false) != 0) return -1;
  Wire.requestFrom(EEPROM_ADDR, 1);
  return Wire.available() ? Wire.read() : -1;
}

// Write only when the value is different: a read costs no wear.
bool update(uint16_t at, uint8_t value) {
  if (readByte(at) == value) return false;
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));
  Wire.write((uint8_t)(at & 0xFF));
  Wire.write(value);
  Wire.endTransmission();
  waitReady();
  writes++;
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  Serial.print("saved setting: ");
  Serial.println(readByte(SETTING_AT));
  Serial.println("type a number from 0 to 255");
}

void loop() {
  if (!Serial.available()) return;
  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0) return;
  int v = line.toInt();
  if (v < 0 || v > 255) {
    Serial.println("0 to 255 only");
    return;
  }
  bool wrote = update(SETTING_AT, v);
  Serial.print(v);
  Serial.print(wrote ? ": saved" : ": unchanged, not written");
  Serial.print("   (writes this run: ");
  Serial.print(writes);
  Serial.println(")");
}
