/*
  Traffic Light - lamp test                             TK03 / /p/tk03

  Wiring. Count from the square pad on the TinkerBlock board, LEDs up,
  header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    RED    -> D9 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an ESP32-S3,
              GP13 on a Raspberry Pi Pico
    YELLOW -> D10 on an Uno, GPIO 26 on an ESP32, GPIO 5 on an ESP32-S3,
              GP14 on a Pico
    GREEN  -> D11 on an Uno, GPIO 27 on an ESP32, GPIO 6 on an ESP32-S3,
              GP15 on a Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// GPIO numbers. Uno: 9, 10, 11. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6.
// Pico: 13, 14, 15.
const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;

// Light one pin for a second and say which it was.
void flash(int pin, const char *name) {
  digitalWrite(pin, HIGH);
  Serial.println(name);
  delay(1000);
  digitalWrite(pin, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);     // each light needs its own pinMode
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  flash(RED_PIN, "RED");
  flash(YELLOW_PIN, "YELLOW");
  flash(GREEN_PIN, "GREEN");

  // All three: the board allows it, a real signal never shows it.
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  Serial.println("all three");
  delay(1000);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  Serial.println("off");
  delay(1000);
}
