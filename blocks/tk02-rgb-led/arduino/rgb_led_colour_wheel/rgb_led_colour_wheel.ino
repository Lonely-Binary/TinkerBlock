/*
  RGB LED - a colour wheel                                TK02 / /p/tk02

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

const unsigned long STEP_MS = 20;   // per degree: 7.2 s per lap

void setColour(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

// hue 0 to 359: red at 0, green at 120, blue at 240.
void wheel(int hue) {
  int sector = hue / 120;
  int up = (hue % 120) * 255 / 120;   // 0 to 252 across the sector
  int down = 255 - up;
  if (sector == 0) setColour(down, up, 0);        // red to green
  else if (sector == 1) setColour(0, down, up);   // green to blue
  else setColour(up, 0, down);                    // blue to red
}

int hue = 0;
unsigned long last = 0;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  if (millis() - last >= STEP_MS) {
    last += STEP_MS;
    wheel(hue);
    hue = (hue + 1) % 360;           // 359 wraps to 0, never 360
  }
  // anything else the sketch does goes here, and never waits
}
