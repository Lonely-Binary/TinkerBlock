// FastLED with WS2812 bar; DATA on one wire controls all LEDs
#include <FastLED.h>

#define DATA_PIN 6   // Bar DATA on D6
#define NUM_LEDS 5   // Number of LEDs (TK33 has 5)

CRGB leds[NUM_LEDS];   // Color array: leds[i] = RGB for LED i

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // Type, pin, color order, array, count
  FastLED.setBrightness(50);   // Global brightness 0-255, avoid too bright
}

void loop() {
  // Running light: each round light LED i, others off; i from 0 to NUM_LEDS-1
  for (int i = 0; i < NUM_LEDS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = CRGB::Black;   // All off first
    }
    leds[i] = CRGB::Red;       // Only LED i red
    FastLED.show();            // Send array to bar so LEDs update
    delay(200);                // 200 ms before next
  }
}
