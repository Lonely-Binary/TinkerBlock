/*
  IR Sender - send a NEC code                            TK16 / /p/tk16

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V for full range (VBUS on a Pico); 3V3 works, shorter
              (VCC only feeds the LEDs, so 5V is safe on any board)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D3 on an Uno, GPIO 22 on an ESP32, GPIO 6 on an
              ESP32-S3, GP17 on a Raspberry Pi Pico

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
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_TX_PIN = 3;

// REPLACE THESE with what the key-code sketch printed for your
// remote. There is no table to look them up in.
const uint16_t ADDRESS = 0x04;
const uint8_t COMMAND = 0x16;
const uint8_t REPEATS = 0;     // 0 is one press; more is a held key

unsigned long lastSend = 0;

void sendOnce() {
  Serial.println("sending");
  // On an Uno the carrier is made in software: let Serial finish
  // first, or its interrupt puts a gap in the tone.
  Serial.flush();
  IrSender.sendNEC(ADDRESS, COMMAND, REPEATS);   // about 68 ms
}

void setup() {
  Serial.begin(115200);
  IrSender.begin(IR_TX_PIN);   // NEC is 38 kHz; nothing else to set
  Serial.println("Sending every 2 s. Type anything to send at once.");
}

void loop() {
  if (Serial.available()) {
    while (Serial.available()) Serial.read();
    lastSend = 0;              // send at once
  }
  if (lastSend == 0 || millis() - lastSend >= 2000) {
    sendOnce();
    lastSend = millis();
  }
}
