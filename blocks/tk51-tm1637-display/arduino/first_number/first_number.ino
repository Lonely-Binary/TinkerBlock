/*
  TM1637 4-Digit Display - the first number        TK51 / /p/tk51

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> your board's logic supply: 5V on an Uno, 3V3 on an
             ESP32 or ESP32-S3. Never 5V beside a 3.3 V board:
             the pull-ups would put 5 V on your pins.
    CLOCK -> D2 on an Uno, GPIO 18 on an ESP32, GPIO 4 on an
             ESP32-S3
    DATA  -> D3 on an Uno, GPIO 19 on an ESP32, GPIO 5 on an
             ESP32-S3

  Arduino IDE
    Tools > Board            your board, e.g. Arduino Uno
    Tools > Port             the one that appears when you plug in
    Tools > USB CDC On Boot  Enabled   (ESP32-S3 only)
    Library: "TM1637" by Avishay Orpaz, from Tools > Manage
    Libraries. It installs TM1637Display.h.
*/

#include <TM1637Display.h>

#define CLOCK_PIN 2    // Uno D2.  ESP32: 18.  ESP32-S3: 4.
#define DATA_PIN  3    // Uno D3.  ESP32: 19.  ESP32-S3: 5.

// CLOCK first, then DATA. The wrong way round is a dark display.
TM1637Display display(CLOCK_PIN, DATA_PIN);

void setup() {
  display.setBrightness(4);     // 0 to 7, sent with the next write
  display.showNumberDec(1234);  // stays there with no further help
  delay(2000);
}

void loop() {
  for (int i = 0; i <= 60; i++) {
    display.showNumberDec(i);   // leading zeros off: 7 is "   7"
    delay(500);
  }
}
