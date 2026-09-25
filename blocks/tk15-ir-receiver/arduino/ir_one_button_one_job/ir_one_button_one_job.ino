/*
  IR Receiver - one button, one job                      TK15 / /p/tk15

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (SIGNAL idles at VCC, so match your board)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> D2 on an Uno, GPIO 23 on an ESP32, GPIO 9 on an
              ESP32-S3, GP16 on a Raspberry Pi Pico

  The LED: a TK01 XL LED (SIGNAL to LED_PIN, GND to GND), or any LED
  with a resistor, on D9, GPIO 4, GPIO 5 or GP15.

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
// A PWM pin for the LED. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

// REPLACE THESE with commands your own remote sent.
const uint16_t CMD_POWER = 0x16;   // toggles: repeats ignored
const uint16_t CMD_UP = 0x17;      // dims up: every repeat counts
const uint16_t CMD_DOWN = 0x18;    // dims down: every repeat counts

bool on = false;
int level = 128;                   // 16 to 255

void show() {
  analogWrite(LED_PIN, on ? level : 0);
  Serial.print(on ? "on  " : "off ");
  Serial.println(level);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK);
  show();
}

void loop() {
  if (!IrReceiver.decode()) return;

  IRData &d = IrReceiver.decodedIRData;
  // A repeat frame carries no data; the library fills in the
  // command of the frame it repeats.
  bool repeat = d.flags & IRDATA_FLAGS_IS_REPEAT;

  if (d.command == CMD_POWER && !repeat) {
    on = !on;                        // once per press
    show();
  } else if (d.command == CMD_UP && on) {
    level = min(level + 16, 255);    // held: keeps going
    show();
  } else if (d.command == CMD_DOWN && on) {
    level = max(level - 16, 16);
    show();
  }

  IrReceiver.resume();    // nothing else decodes until this runs
}
