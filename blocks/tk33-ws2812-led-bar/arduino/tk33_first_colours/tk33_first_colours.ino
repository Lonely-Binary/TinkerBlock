// TK33 WS2812 RGB LED Bar: the first colours, on an ESP32-S3.
//
// Wiring, TK33 header top to bottom (LEDs facing you, pins left):
//   GND  -> ESP32 GND
//   VCC  -> ESP32 5V     (not 3V3: the LEDs want 3.5 V or more)
//   NC   -> nothing
//   DATA -> GPIO4
// Make the wires with the USB cable unplugged.
//
// Arduino IDE: Tools > Board > esp32 > ESP32S3 Dev Module,
// Tools > USB CDC On Boot > Enabled, then Tools > Port.
// Library Manager: install "Adafruit NeoPixel".
// Serial Monitor at 115200.

#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 4;
const int LEDS = 5;      // five on this bar, and nothing after them

// NEO_GRB: this LED reads green first. NEO_KHZ800: 800 kbit/s.
Adafruit_NeoPixel bar(LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// Top to bottom at the start: red, green, blue, white, dark.
const uint32_t COLOUR[LEDS] = {
  0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0x000000
};
const char *NAME[LEDS] = { "red", "green", "blue", "white", "dark" };

int step = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  bar.begin();
  bar.setBrightness(40);  // of 255: 30 mA at most, all five white
  bar.show();             // a dark frame clears anything from power-up
}

void loop() {
  // Each colour moves one LED down per second; LED1 is pixel 0.
  for (int i = 0; i < LEDS; i++) {
    bar.setPixelColor(i, COLOUR[(i + LEDS - step) % LEDS]);
  }
  bar.show();             // 120 bits, then the line rests low

  Serial.printf("LED1 (top) should be %s\n",
                NAME[(LEDS - step) % LEDS]);
  step = (step + 1) % LEDS;
  delay(1000);
}
