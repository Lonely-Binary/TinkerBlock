// Keymap finder: find the adapter, then press every key once in printed
// order, and it prints the keymap for your keypad.
//
// Wiring, adapter to board (as in "The first keypress"):
//   Adapter GND -> board GND
//   Adapter VCC -> board 5V on an Uno, 3V3 on an ESP32 or ESP32-S3
//   Adapter SDA -> Uno A4, ESP32 GPIO 21, ESP32-S3 GPIO 8
//   Adapter SCL -> Uno A5, ESP32 GPIO 22, ESP32-S3 GPIO 9
//
// Arduino IDE: install "I2CKeyPad" by Rob Tillaart from the Library
// Manager. No special Tools settings. Serial Monitor at 115200.
// Unplug any I2C screen first, so the scan cannot mistake it for the keypad.

#include <Wire.h>
#include "I2CKeyPad.h"

const char ORDER[] = "123A456B789C*0#D";   // the order it asks for keys

// PCF8574 answers at 0x20-0x27, PCF8574A at 0x38-0x3F.
uint8_t findAdapter() {
  for (uint8_t a = 0x20; a <= 0x3F; a++) {
    if (a == 0x28) a = 0x38;
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) return a;
  }
  return 0;
}

// Wait for exactly one key, then for it to be let go.
uint8_t waitForKey(I2CKeyPad &kp) {
  uint8_t k;
  do { k = kp.getKey(); delay(20); } while (k > 15);
  while (kp.getKey() != I2C_KEYPAD_NOKEY) delay(20);
  return k;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin();
  Wire.setClock(100000);

  uint8_t addr = findAdapter();
  if (addr == 0) {
    Serial.println("No adapter at 0x20-0x27 or 0x38-0x3F. Check GND, VCC, SDA, SCL.");
    return;
  }
  Serial.print("Adapter at 0x");
  Serial.println(addr, HEX);

  I2CKeyPad kp(addr);
  kp.begin();

  char found[19] = "????????????????NF";
  for (int i = 0; i < 16; i++) {
    Serial.print("Press ");
    Serial.println(ORDER[i]);
    uint8_t k = waitForKey(kp);
    Serial.print("  key number ");
    Serial.println(k);
    found[k] = ORDER[i];
  }
  Serial.print("\nchar keymap[19] = \"");
  Serial.print(found);
  Serial.println("\";");
}

void loop() {}
