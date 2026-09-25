/*
  XL LED - fade with analogWrite                          TK01 / /p/tk01

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

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 0 is always LOW, 255 is always HIGH; in between, the pin spends
  // that fraction of every PWM period HIGH.
  for (int duty = 0; duty <= 255; duty++) {
    analogWrite(LED_PIN, duty);
    delay(8);
  }
  for (int duty = 255; duty >= 0; duty--) {
    analogWrite(LED_PIN, duty);
    delay(8);
  }
}
