/*
  RGB LED - mixing with PWM                               TK02 / /p/tk02

  Wiring. Count from the square pad on the TinkerBlock board, LED side
  up, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    NC     -> nothing
    RED    -> a PWM pin: D9 on an Uno, GPIO 25 on an ESP32,
              GPIO 4 on an ESP32-S3, GP13 on a Raspberry Pi Pico
    GREEN  -> D10, GPIO 26, GPIO 5, GP14
    BLUE   -> D11, GPIO 27, GPIO 6, GP15

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed. ESP32 boards need board package 2.0 or later.
*/

// Uno: 9, 10, 11. ESP32: 25, 26, 27. ESP32-S3: 4, 5, 6. Pico: 13, 14, 15.
const int RED_PIN = 4;
const int GREEN_PIN = 5;
const int BLUE_PIN = 6;

// 0 is off, 255 is full. Common cathode: no "255 -" anywhere.
void setColour(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void show(int r, int g, int b) {
  setColour(r, g, b);
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.println(b);
  delay(1500);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  show(255, 255, 0);    // red and green
  show(0, 255, 255);    // green and blue
  show(255, 0, 255);    // blue and red
  show(255, 60, 0);     // mostly red, a little green
  show(255, 255, 255);  // all three full: look closely
}
