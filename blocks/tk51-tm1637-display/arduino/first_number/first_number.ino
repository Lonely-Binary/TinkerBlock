// The first number: 1234, then a count from 0 to 60.
//
// Wiring, display to board:
//   Display GND   -> board GND
//   Display VCC   -> board 5V on an Uno, 3V3 on an ESP32 or ESP32-S3
//   Display CLOCK -> Uno D2, ESP32 GPIO 18, ESP32-S3 GPIO 4
//   Display DATA  -> Uno D3, ESP32 GPIO 19, ESP32-S3 GPIO 5
//
// Arduino IDE: install "TM1637" by Avishay Orpaz from the Library Manager
// (Tools -> Manage Libraries, search TM1637). No special Tools settings on
// any of the three boards.

#include <TM1637Display.h>

#define CLOCK_PIN 2    // Uno D2.  ESP32: 18.  ESP32-S3: 4.
#define DATA_PIN  3    // Uno D3.  ESP32: 19.  ESP32-S3: 5.

// CLOCK first, then DATA. Getting these the wrong way round is the most
// common reason a display stays dark.
TM1637Display display(CLOCK_PIN, DATA_PIN);

void setup() {
  display.setBrightness(4);       // 0 to 7; only takes effect on the next write
  display.showNumberDec(1234);    // and it stays there, with no further help
  delay(2000);
}

void loop() {
  for (int i = 0; i <= 60; i++) {
    display.showNumberDec(i);     // leading zeros off: 7 shows as "   7"
    delay(500);
  }
}
