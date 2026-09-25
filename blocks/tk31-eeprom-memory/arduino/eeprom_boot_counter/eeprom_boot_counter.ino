/*
  EEPROM Memory - boot counter                          TK31 / /p/tk31

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

const int EEPROM_ADDR = 0x50;  // fixed: A0 to A2 are tied to GND
const uint16_t COUNT_AT = 0;   // the byte that holds the count

// Returns true if the chip acknowledged every byte.
bool writeByte(uint16_t at, uint8_t value) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));     // memory address, high byte
  Wire.write((uint8_t)(at & 0xFF));   // then the low byte
  Wire.write(value);
  bool ok = Wire.endTransmission() == 0;  // the stop starts the write
  delay(5);                           // up to 5 ms, answering nobody
  return ok;
}

int readByte(uint16_t at) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));
  Wire.write((uint8_t)(at & 0xFF));
  if (Wire.endTransmission(false) != 0) return -1;  // no answer
  Wire.requestFrom(EEPROM_ADDR, 1);   // repeated start, then read
  return Wire.available() ? Wire.read() : -1;
}

void setup() {
  Serial.begin(115200);
  delay(1000);                        // time for the monitor to open
  Wire.begin();                       // the board's default I2C pins

  int count = readByte(COUNT_AT);
  if (count < 0) {
    Serial.println("no answer at 0x50: check the four wires");
    return;
  }
  if (count == 255) count = 0;        // a byte never written
  count++;
  writeByte(COUNT_AT, count);

  Serial.print("Started ");
  Serial.print(count);
  Serial.println(" times. Unplug it, plug it back in.");
}

void loop() {
}
