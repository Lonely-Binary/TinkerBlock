/*
  RGB LED - equal numbers against white                   TK02 / /p/tk02

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

// A first guess at white. 3.3 V board: 84, 255, 122. Uno: 167, 255, 122.
const int WHITE[3] = {84, 255, 122};

void setColour(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  setColour(255, 255, 255);
  Serial.println("255, 255, 255: equal numbers");
  delay(2000);

  setColour(WHITE[0], WHITE[1], WHITE[2]);
  Serial.println("WHITE: balanced");
  delay(2000);
}
