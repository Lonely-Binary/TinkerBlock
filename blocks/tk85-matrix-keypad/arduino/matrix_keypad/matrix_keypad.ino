#include <Wire.h>
#include "I2CKeyPad.h"

// 16 keys, then N (no key) and F (fail). Pick the one for your keypad.
char keymap[19] = "DCBA#9630852*741NF";      // soft keypad
// char keymap[19] = "D#0*C987B654A321NF";   // heavy-duty keypad

I2CKeyPad keyPad(0x20);   // fixed on the TK85; the compact adapter ships here
char last = 'N';

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);   // the PCF8574 is rated for 100 kHz
  if (!keyPad.begin()) {
    Serial.println("No keypad at 0x20");
    while (true) delay(1000);
  }
  keyPad.loadKeyMap(keymap);
}

void loop() {
  char c = keyPad.getChar();
  if (c != last) {
    if (c != 'N' && c != 'F') Serial.println(c);
    last = c;
  }
  delay(20);
}
