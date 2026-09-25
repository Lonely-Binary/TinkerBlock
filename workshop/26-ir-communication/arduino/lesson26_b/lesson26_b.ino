// Program 2: IR sender (upload to board B with TK16 on D3)
#include <IRremote.hpp>

#define IR_TX_PIN 3   // TK16 SIGNAL on D3

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_TX_PIN);                // Start sending on D3
  Serial.println("IR sender ready");
}

void loop() {
  Serial.println("IR sent");
  Serial.flush();                           // Let printing finish first
  IrSender.sendNEC(0x04, 0x16, 0);          // Address 0x04, command 0x16, no repeats
  delay(2000);                              // Every 2 s so the receiver can see
}
