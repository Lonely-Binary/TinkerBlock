/*
  Infrared Transmitter - to a TK15            TK63 + TK15 / /p/tk63

  Wiring. Count from the square pad on each TinkerBlock board, parts
  up, header at the bottom:

    TK63 transmitter
      GND    -> GND
      VCC    -> 5V, or VBUS on a Pico (only feeds the LEDs)
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> D3, GPIO 22, GPIO 6 or GP17

    TK15 receiver
      GND    -> GND
      VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
      NC     -> nothing   (unconnected on the board)
      SIGNAL -> D2, GPIO 23, GPIO 9 or GP16

  Uno, ESP32, ESP32-S3, Pico, in that order. Stand the two blocks
  face to face, about 30 cm apart.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Manage Libraries      IRremote by shirriff, z3t0 and
                                  ArminJo, version 4 or later
    Serial Monitor                115200
*/

#include <IRremote.hpp>

// The pin the TK63's SIGNAL is wired to.
// Uno: 3. ESP32: 22. ESP32-S3: 6. Pico: 17.
const int IR_PIN = 6;
// The pin the TK15's SIGNAL is wired to.
// Uno: 2. ESP32: 23. ESP32-S3: 9. Pico: 16.
const int RX_PIN = 9;

// Any address nothing else in the room is likely to send. The
// command counts up, so each line shows a new frame arriving.
const uint16_t ADDRESS = 0x12;
uint8_t command = 0;

void setup() {
  // LOW first: nothing on the board holds the transistor off.
  pinMode(IR_PIN, OUTPUT);
  digitalWrite(IR_PIN, LOW);
  Serial.begin(115200);
  // IRremote makes the 38 kHz itself: on an Uno in software, on
  // an ESP32, ESP32-S3 or Pico with a hardware PWM channel.
  IrSender.begin(IR_PIN);
  IrReceiver.begin(RX_PIN, DISABLE_LED_FEEDBACK);
  Serial.println("Sending a NEC frame once a second.");
}

void loop() {
  Serial.print("sent ");
  Serial.print(command);
  Serial.flush();                  // no interrupt mid-frame on an Uno
  IrSender.sendNEC(ADDRESS, command, 0);   // about 68 ms

  // Listen again, and wait long enough to see the frame has ended.
  IrReceiver.restartAfterSend();
  delay(RECORD_GAP_MICROS / 1000 + 5);

  if (IrReceiver.decode()) {
    IRData &d = IrReceiver.decodedIRData;
    if (d.address == ADDRESS && d.command == command) {
      Serial.println("  heard it");
    } else if (d.protocol == UNKNOWN) {
      Serial.println("  heard something, decoded nothing");
    } else {
      Serial.println("  heard another frame");
    }
    IrReceiver.resume();
  } else {
    Serial.println("  nothing heard");
  }

  command++;
  delay(1000);
}
