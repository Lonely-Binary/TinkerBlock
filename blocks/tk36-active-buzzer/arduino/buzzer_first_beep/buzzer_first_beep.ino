/*
  Active Buzzer - first beep                            TK36 / /p/tk36

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V: an Uno's 5V, the 5V pin of an ESP32 or ESP32-S3
              board on USB, a Pico's VBUS (3V3 works, quieter)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3 or ESP32, D9 on an Uno,
              GP15 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.

  Peel the paper seal off the buzzer first: it covers the sound hole.
*/

// The pin SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 4. Pico: 15.
const int BUZZER_PIN = 4;

void setup() {
  // First, before anything else: the board has no pull-down, so
  // SIGNAL is whatever this pin is until the sketch drives it.
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.begin(115200);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);   // transistor on: beep
  Serial.println("beep");
  delay(200);

  digitalWrite(BUZZER_PIN, LOW);    // off: silence
  delay(800);
}
