/*
  IR loop-back test                         TK15 + TK16 / /p/tbir

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK15 receiver
      GND    -> GND
      VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> D2, GPIO 23, GPIO 9 or GP16

    TK16 sender
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> D3, GPIO 22, GPIO 6 or GP17

  Uno, ESP32, ESP32-S3, Pico, in that order.

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      IRremote by shirriff, z3t0 and
                                  ArminJo, version 4 or later
    Serial Monitor                115200
*/

#include <IRremote.hpp>

// Uno: 2. ESP32: 23. ESP32-S3: 9. Pico: 16.
const int IR_RX_PIN = 2;
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_TX_PIN = 3;

// Any pair nothing else in the room is likely to send.
const uint16_t ADDRESS = 0x12;
const uint8_t COMMAND = 0x34;

unsigned long sent = 0, back = 0;

void setup() {
  Serial.begin(115200);
  IrSender.begin(IR_TX_PIN);
  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK);
  Serial.println("Loop-back: sending once a second.");
}

void loop() {
  Serial.flush();                  // no interrupt mid-frame on an Uno
  IrSender.sendNEC(ADDRESS, COMMAND, 0);
  sent++;

  // The library's own SendAndReceive order: restart the receiver,
  // then wait long enough for it to see that the frame has ended.
  IrReceiver.restartAfterSend();
  delay(RECORD_GAP_MICROS / 1000 + 5);

  if (IrReceiver.decode()) {
    IRData &d = IrReceiver.decodedIRData;
    if (d.protocol == UNKNOWN) {
      Serial.println("heard something, decoded nothing: move apart");
    } else if (d.address == ADDRESS && d.command == COMMAND) {
      back++;
      Serial.print("ok  ");
    } else {
      Serial.print("someone else's frame  ");
    }
    IrReceiver.resume();
  } else {
    Serial.print("nothing came back  ");
  }
  Serial.print(back);
  Serial.print(" of ");
  Serial.println(sent);
  delay(1000);
}
