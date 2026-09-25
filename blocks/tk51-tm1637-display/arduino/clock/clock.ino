/*
  TM1637 4-Digit Display - a clock that sets itself  TK51 / /p/tk51

  An ESP32 and a clock panel. The time comes over Wi-Fi from NTP.

  Wiring. No pad on this board is square, so go by the names
  printed on the back: GND, VCC, clock, data. Seen from the
  display side with the header on the left, GND is the top pin.

    GND   -> GND
    VCC   -> 3V3. Never 5V beside a 3.3 V board: the pull-ups
             would put 5 V on your pins.
    CLOCK -> GPIO 18
    DATA  -> GPIO 19

  Arduino IDE
    Tools > Board            ESP32 Dev Module
    Tools > Port             the one that appears when you plug in
    Library: "TM1637" by Avishay Orpaz, from Tools > Manage
    Libraries. WiFi and time.h come with the ESP32 core.
    Serial Monitor at 115200.
*/

#include <WiFi.h>
#include <time.h>
#include <TM1637Display.h>

#define CLOCK_PIN 18
#define DATA_PIN  19
#define COLON     0b01000000   // bit 7 of the second digit

const char *SSID     = "your-ssid";
const char *PASSWORD = "your-password";

// POSIX timezone: the offset and the two days daylight saving
// changes on, so the board makes the switch itself. This one is
// Melbourne. Replace it with your own.
const char *TZ = "AEST-10AEDT,M10.1.0,M4.1.0/3";

TM1637Display display(CLOCK_PIN, DATA_PIN);

// Shown until the time is real. A wrong time looks exactly like a
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
    display.setSegments(DASHES);   // no time yet
    delay(500);
    return;
  }

  // Leading zeros on. Without them, midnight sends the number 0,
  // and the released library drops the colon with the blanks.
  display.showNumberDecEx(t.tm_hour * 100 + t.tm_min,
                          (t.tm_sec % 2) ? COLON : 0,
                          true);
  delay(200);
}
