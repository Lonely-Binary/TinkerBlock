/*
  Mechanical Key and LED - a key that remembers          TK96 / /p/tk96

  Each press moves to the next mode, and the key shows the mode it
  is in: off, red, green, blue, then off again.

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

// Uno: 2. ESP32: 25. ESP32-S3: 4. Pico: 15.
const int BUTTON_PIN = 4;
// Uno: 6. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LED_PIN = 5;
const unsigned long DEBOUNCE_MS = 20;

Adafruit_NeoPixel key(1, LED_PIN, NEO_GRB + NEO_KHZ800);

const int MODES = 4;
const uint32_t COLOUR[MODES] = { 0x000000, 0xFF0000, 0x00FF00,
                                 0x0000FF };
const char *NAME[MODES] = { "off", "red", "green", "blue" };

int mode = 0;                   // what the key remembers
int lastReading = LOW;
int state = LOW;
unsigned long lastChange = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);   // the block has its own pull-down
  key.begin();
  key.show();                   // a dark frame: mode 0 is off
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (reading != lastReading) { // the contacts moved: restart the clock
    lastReading = reading;
    lastChange = now;
  }

  if (now - lastChange >= DEBOUNCE_MS && reading != state) {
    state = reading;
    if (state == HIGH) {        // one real press: next mode
      mode = (mode + 1) % MODES;
      key.setPixelColor(0, COLOUR[mode]);
      key.show();
      Serial.print("mode ");
      Serial.print(mode);
      Serial.print("  ");
      Serial.println(NAME[mode]);
    }
  }
}
