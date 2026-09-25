/*
  RGB LED - the first colour                              TK02 / /p/tk02

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    NC     -> nothing
    RED    -> D9 on an Uno, GPIO 25 on an ESP32,
              GPIO 4 on an ESP32-S3, GP13 on a Raspberry Pi Pico
    GREEN  -> D10, GPIO 26, GPIO 5, GP14
    BLUE   -> D11, GPIO 27, GPIO 6, GP15

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// Uno: 9, 10, 11. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
const int RED_PIN = 4;
const int GREEN_PIN = 5;
const int BLUE_PIN = 6;

void show(bool r, bool g, bool b, const char *name) {
  digitalWrite(RED_PIN, r ? HIGH : LOW);   // HIGH lights: common cathode
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN, b ? HIGH : LOW);
  Serial.println(name);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  show(true, false, false, "red");
  show(false, true, false, "green");
  show(false, false, true, "blue");
  show(false, false, false, "off");
}
