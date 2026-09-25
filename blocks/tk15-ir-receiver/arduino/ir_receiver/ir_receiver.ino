#include <IRremote.hpp>   // IRremote by shirriff, z3t0 and ArminJo

// Uno: 2. ESP32: 23. ESP32-S3: 9. Pico: 16.
const int IR_RX_PIN = 2;

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK);
}

void loop() {
  if (!IrReceiver.decode()) return;
  IRData &d = IrReceiver.decodedIRData;
  if (d.flags & IRDATA_FLAGS_IS_REPEAT) {
    Serial.println("[repeat]");        // a held button
  } else {
    Serial.print("address=0x");
    Serial.print(d.address, HEX);
    Serial.print("  command=0x");
    Serial.println(d.command, HEX);
  }
  IrReceiver.resume();    // nothing else decodes until this runs
}
