#include <TM1637Display.h>

#define CLOCK_PIN 2   // Uno D2.  ESP32: 18.  ESP32-S3: 4.
#define DATA_PIN  3   // Uno D3.  ESP32: 19.  ESP32-S3: 5.
#define COLON     0b01000000   // bit 7 of the second digit

// CLOCK first, then DATA.
TM1637Display display(CLOCK_PIN, DATA_PIN);

void setup() {
  display.setBrightness(4);          // 0 to 7, sent with the next lot of digits
  display.showNumberDec(1234);       // stays there with no further help
  delay(2000);
}

void loop() {
  // 12:34 on a clock panel, 12.34 on a digit panel — the same four bytes.
  // Leading zeros on, or a number below 1000 loses its leading digits.
  display.showNumberDecEx(1234, COLON, true);
  delay(1000);
  display.showNumberDecEx(1234, 0, true);
  delay(1000);
}
