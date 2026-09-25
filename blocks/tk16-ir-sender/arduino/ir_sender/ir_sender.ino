#include <IRremote.hpp>   // IRremote by shirriff, z3t0 and ArminJo

// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_TX_PIN = 3;

const uint16_t ADDRESS = 0x04;   // REPLACE: from your own remote
const uint8_t COMMAND = 0x16;    // REPLACE: from your own remote

void setup() {
  Serial.begin(115200);
  IrSender.begin(IR_TX_PIN);
}

void loop() {
  Serial.flush();        // on an Uno, no interrupt mid-frame
  IrSender.sendNEC(ADDRESS, COMMAND, 0);   // 0 repeats: one press
  Serial.println("sent");
  delay(2000);
}
