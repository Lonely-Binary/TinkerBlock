/*
  IR Receiver - key codes                                TK15 / /p/tk15

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (SIGNAL idles at VCC, so match your board)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 23 on an ESP32, GPIO 9 on an
              ESP32-S3, GP16 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      IRremote by shirriff, z3t0 and
                                  ArminJo, version 4 or later
    Serial Monitor                115200
*/

#include <IRremote.hpp>

// The pin SIGNAL is wired to.
// Uno: 2. ESP32: 23. ESP32-S3: 9. Pico: 16.
const int IR_RX_PIN = 2;

// Every address-and-command pair seen so far.
const int MAX_KEYS = 32;
uint32_t seen[MAX_KEYS];
int keys = 0;

bool firstTime(uint32_t key) {
  for (int i = 0; i < keys; i++) {
    if (seen[i] == key) return false;
  }
  if (keys < MAX_KEYS) seen[keys++] = key;
  return true;
}

void setup() {
  Serial.begin(115200);
  // DISABLE_LED_FEEDBACK: leave your board's own LED alone.
  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK);
  Serial.println("Press every button on the remote.");
}

void loop() {
  if (!IrReceiver.decode()) return;     // no whole frame yet

  IRData &d = IrReceiver.decodedIRData;
  if (d.flags & IRDATA_FLAGS_IS_REPEAT) {
    Serial.println("[repeat]");         // a held button
  } else if (d.protocol == UNKNOWN) {
    Serial.println("UNKNOWN: no decoder matched this frame");
  } else {
    Serial.print(getProtocolString(d.protocol));
    Serial.print("  address=0x");
    Serial.print(d.address, HEX);
    Serial.print("  command=0x");
    Serial.print(d.command, HEX);
    uint32_t key = ((uint32_t)d.address << 16) | d.command;
    if (firstTime(key)) {
      Serial.print("  first time, ");
      Serial.print(keys);
      Serial.print(" so far");
    }
    Serial.println();
  }

  IrReceiver.resume();    // nothing else decodes until this runs
}
