/*
  Traffic Light - first sequence, with delay()          TK03 / /p/tk03

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

// Set all three at once, so no step can leave a light on by mistake.
void show(bool red, bool yellow, bool green) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  show(HIGH, LOW, LOW);         // red: stop
  Serial.println("red");
  delay(5000);

  show(LOW, LOW, HIGH);         // green: go
  Serial.println("green");
  delay(5000);

  show(LOW, HIGH, LOW);         // yellow: about to turn red
  Serial.println("yellow");
  delay(2000);
}
