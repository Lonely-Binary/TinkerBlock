/*
  Mechanical Key and LED - the first press               TK96 / /p/tk96

  Wiring. Count from the square pad on the TinkerBlock board, key
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (pressed, BUTTON gives your pin whatever VCC is)
    WS2812 -> D6 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP14 on a Raspberry Pi Pico
    BUTTON -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Library Manager: install "Adafruit NeoPixel".
*/

#include <Adafruit_NeoPixel.h>

// The GPIO number BUTTON is wired to.
// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUTTON_PIN = 4;
// The GPIO number the WS2812 pin is wired to.
// Uno: 6. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;

// One LED on the block. It reads green first, at 800 kbit/s.
Adafruit_NeoPixel key(1, LED_PIN, NEO_GRB + NEO_KHZ800);

int last = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
  key.begin();
  key.show();                   // a dark frame: the LED starts unset
}

void loop() {
  int level = digitalRead(BUTTON_PIN);

  if (level != last) {          // print only when it changes
    last = level;
    if (level == HIGH) {
      key.setPixelColor(0, key.Color(255, 0, 0));   // red
      Serial.println("HIGH  pressed");
    } else {
      key.setPixelColor(0, 0);                      // off
      Serial.println("LOW   released");
    }
    key.show();                 // nothing changes until show()
  }
  delay(5);
}
