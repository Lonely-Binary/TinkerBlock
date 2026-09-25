#include <Wire.h>

const int EEPROM_ADDR = 0x50;   // fixed on the TK31

void writeByte(uint16_t at, uint8_t value) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));     // address, high byte
  Wire.write((uint8_t)(at & 0xFF));   // then low byte
  Wire.write(value);
  Wire.endTransmission();
  delay(5);                           // the chip is writing
}

int readByte(uint16_t at) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));
  Wire.write((uint8_t)(at & 0xFF));
  Wire.endTransmission(false);
  Wire.requestFrom(EEPROM_ADDR, 1);
  return Wire.available() ? Wire.read() : -1;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();                       // the board's default I2C pins
  writeByte(0, 123);
  Serial.println(readByte(0));        // 123, before and after power-off
}

void loop() {
}
