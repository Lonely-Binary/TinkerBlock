/*
  EEPROM Memory - how long a write takes                TK31 / /p/tk31

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

// Send a write, and return as soon as the stop has gone.
void sendByte(uint16_t at, uint8_t value) {
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(at >> 8));
  Wire.write((uint8_t)(at & 0xFF));
  Wire.write(value);
  Wire.endTransmission();
}

// Is the chip there? 0 from endTransmission means it acknowledged.
bool answers() {
  Wire.beginTransmission(EEPROM_ADDR);
  return Wire.endTransmission() == 0;
}

// Acknowledge polling: ask until it answers, at most 20 ms.
unsigned long waitReady() {
  unsigned long t0 = micros();
  while (!answers() && micros() - t0 < 20000) {
  }
  return micros() - t0;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  sendByte(100, 42);
  Wire.beginTransmission(EEPROM_ADDR);
  Serial.print("asked straight away: endTransmission() = ");
  Serial.println(Wire.endTransmission());   // not 0: nobody answered
  waitReady();

  for (int i = 0; i < 5; i++) {
    sendByte(100 + i, i);
    unsigned long us = waitReady();
    Serial.print("write ");
    Serial.print(i);
    Serial.print(" was done after ");
    Serial.print(us);
    Serial.println(" us");
  }
}

void loop() {
}
