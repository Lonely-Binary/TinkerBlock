// The first keypress: print every key once, as it goes down.
//
// Wiring, adapter to board:
//   Adapter GND -> board GND
//   Adapter VCC -> board 5V on an Uno, 3V3 on an ESP32 or ESP32-S3
//   Adapter SDA -> Uno A4, ESP32 GPIO 21, ESP32-S3 GPIO 8
//   Adapter SCL -> Uno A5, ESP32 GPIO 22, ESP32-S3 GPIO 9
//
// Arduino IDE: install "I2CKeyPad" by Rob Tillaart from the Library
// Manager. Any of the three boards, no special Tools settings.
// Serial Monitor at 115200.

#include <Wire.h>
#include "I2CKeyPad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;   // as shipped

// 16 keys, then N (no key) and F (fail). Pick the one for your keypad.
char keymap[19] = "DCBA#9630852*741NF";      // soft keypad: 5-pack, TinkerBlock
// char keymap[19] = "D#0*C987B654A321NF";   // heavy-duty keypad: 2-pack

I2CKeyPad keyPad(KEYPAD_ADDRESS);
char last = 'N';

void setup() {
  Serial.begin(115200);
  delay(500);
  Wire.begin();
  Wire.setClock(100000);   // the PCF8574 is rated for 100 kHz
  if (!keyPad.begin()) {
    Serial.println("No keypad at 0x20 - check GND, VCC, SDA and SCL.");
    while (true) delay(1000);
  }
  keyPad.loadKeyMap(keymap);
  Serial.println("Ready. Press a key.");
}

void loop() {
  char c = keyPad.getChar();
  if (c != last) {                       // only when something changed
    if (c != 'N' && c != 'F') Serial.println(c);
    last = c;
  }
  delay(20);
}
