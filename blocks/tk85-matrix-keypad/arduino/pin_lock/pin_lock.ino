// A four-digit code lock: type the code, press #, and a pin goes HIGH.
//
// Wiring, adapter to board (as in "The first keypress"):
//   Adapter GND -> board GND
//   Adapter VCC -> board 5V on an Uno, 3V3 on an ESP32 or ESP32-S3
//   Adapter SDA -> Uno A4, ESP32 GPIO 21, ESP32-S3 GPIO 8
//   Adapter SCL -> Uno A5, ESP32 GPIO 22, ESP32-S3 GPIO 9
//   OPEN_PIN    -> the board's LED, or an LED and 220 ohm resistor to GND
//
// Arduino IDE: install "I2CKeyPad" by Rob Tillaart from the Library
// Manager. No special Tools settings. Serial Monitor at 115200.

#include <Wire.h>
#include "I2CKeyPad.h"

#define CODE        "2580"
#define TRIES       3
#define LOCKOUT_MS  10000UL
#define OPEN_MS     3000UL

const int OPEN_PIN = LED_BUILTIN;

char keymap[19] = "DCBA#9630852*741NF";      // soft keypad
// char keymap[19] = "D#0*C987B654A321NF";   // heavy-duty keypad

I2CKeyPad keyPad(0x20);
char last = 'N';
String entry;
int triesLeft = TRIES;

void setup() {
  Serial.begin(115200);
  pinMode(OPEN_PIN, OUTPUT);
  digitalWrite(OPEN_PIN, LOW);
  Wire.begin();
  Wire.setClock(100000);
  if (!keyPad.begin()) {
    Serial.println("No keypad at 0x20 - check GND, VCC, SDA and SCL.");
    while (true) delay(1000);
  }
  keyPad.loadKeyMap(keymap);
  Serial.println("Locked. Type the code, then #.");
}

void check() {
  if (entry == CODE) {
    Serial.println("OPEN");
    digitalWrite(OPEN_PIN, HIGH);
    delay(OPEN_MS);                  // keys are ignored while open
    digitalWrite(OPEN_PIN, LOW);
    triesLeft = TRIES;
    Serial.println("Locked.");
  } else if (--triesLeft > 0) {
    Serial.print("Wrong. Tries left: ");
    Serial.println(triesLeft);
  } else {
    Serial.println("Too many tries. Locked out.");
    delay(LOCKOUT_MS);               // and while locked out
    triesLeft = TRIES;
    Serial.println("Try again.");
  }
  entry = "";
}

void loop() {
  char c = keyPad.getChar();
  if (c != last) {
    last = c;
    if (c >= '0' && c <= '9' && entry.length() < 8) {
      entry += c;
      Serial.print('*');
    } else if (c == '*') {
      entry = "";
      Serial.println(" cleared");
    } else if (c == '#') {
      Serial.println();
      check();
    }
  }
  delay(20);
}
