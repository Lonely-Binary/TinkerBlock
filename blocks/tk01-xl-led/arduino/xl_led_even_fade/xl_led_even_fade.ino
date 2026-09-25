/*
  XL LED - a fade that looks even                         TK01 / /p/tk01

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIGNAL -> a PWM pin: D9 on an Uno, GPIO 4 on an ESP32 or ESP32-S3,
              GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed. ESP32 boards need board package 2.0 or later.
*/

// A PWM pin. Uno: 9. ESP32, ESP32-S3: 4. Pico: 15.
const int LED_PIN = 4;

// 32 steps that look equally spaced: 255 * (i / 31) ^ 2.2, rounded.
const uint8_t LOOKS_EVEN[32] = {
    0,   0,   1,   1,   3,   5,   7,  10,
   13,  17,  21,  26,  32,  38,  44,  52,
   60,  68,  77,  87,  97, 108, 120, 132,
  145, 159, 173, 188, 204, 220, 237, 255
};

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < 32; i++) {
    analogWrite(LED_PIN, LOOKS_EVEN[i]);
    delay(60);
  }
  for (int i = 31; i >= 0; i--) {
    analogWrite(LED_PIN, LOOKS_EVEN[i]);
    delay(60);
  }
}
