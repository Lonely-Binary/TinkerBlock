// A clock that sets itself: ESP32 + a TM1637 clock panel, time over Wi-Fi.
//
// Wiring, display to board:
//   Display GND   -> board GND
//   Display VCC   -> board 3V3   (not 5V: the board's pull-ups would then
//                                 hold CLOCK and DATA at 5 V against pins
//                                 rated for 3.3 V)
//   Display CLOCK -> GPIO 18
//   Display DATA  -> GPIO 19
//
// Arduino IDE: ESP32 board support installed, board "ESP32 Dev Module",
// and "TM1637" by Avishay Orpaz from the Library Manager. Serial Monitor
// at 115200.

#include <WiFi.h>
#include <time.h>
#include <TM1637Display.h>

#define CLOCK_PIN 18
#define DATA_PIN  19
#define COLON     0b01000000   // bit 7 of the second digit

const char *SSID     = "your-ssid";
const char *PASSWORD = "your-password";

// POSIX timezone: the offset and the two dates daylight saving changes on,
// so the board handles the switch itself. Replace with your own.
const char *TZ = "AEST-10AEDT,M10.1.0,M4.1.0/3";

TM1637Display display(CLOCK_PIN, DATA_PIN);

// Shown until the time is real. An old or wrong time looks identical to a
// right one; four dashes do not.
const uint8_t DASHES[] = { 0x40, 0x40, 0x40, 0x40 };

void setup() {
  Serial.begin(115200);
  display.setBrightness(4);
  display.setSegments(DASHES);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(250);
  Serial.println(WiFi.localIP());

  configTzTime(TZ, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  struct tm t;
  if (!getLocalTime(&t, 100) || t.tm_year + 1900 < 2020) {
    display.setSegments(DASHES);       // no time yet
    delay(500);
    return;
  }

  // Leading zeros on: without them, midnight sends the number 0 and this
  // version of the library drops the colon along with the blank digits.
  display.showNumberDecEx(t.tm_hour * 100 + t.tm_min,
                          (t.tm_sec % 2) ? COLON : 0,
                          true);
  delay(200);
}
