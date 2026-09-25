/*
  4-Direction Tilt Sensor - first read                   TK19 / /p/tk19

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
            (every LED hangs from VCC onto a line your pin holds)
    A    -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
            ESP32-S3, GP10 on a Raspberry Pi Pico
    B    -> D3, GPIO 26, GPIO 5, GP11      (same order)
    C    -> D4, GPIO 27, GPIO 6, GP12
    D    -> D5, GPIO 32, GPIO 7, GP13

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The GPIO numbers A, B, C and D are wired to.
// Uno: 2 3 4 5. ESP32: 25 26 27 32. ESP32-S3: 4 5 6 7. Pico: 10 11 12 13.
const int PIN_A = 4;
const int PIN_B = 5;
const int PIN_C = 6;
const int PIN_D = 7;

// Which edge is down. NO_SIDE: the roller is joining no pair.
const int NO_SIDE = 0;
const int LEFT_DOWN = 1;
const int TOP_DOWN = 2;
const int RIGHT_DOWN = 3;
const int HEADER_DOWN = 4;
const char* SIDE_NAME[] = {"none", "left", "top", "right", "header"};

// Every line an input with its pull-up. No line is ever driven HIGH.
void releaseAll() {
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_C, INPUT_PULLUP);
  pinMode(PIN_D, INPUT_PULLUP);
}

// Hold one line LOW: LOW first, then output, so it never goes HIGH.
void holdLow(int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

bool readsLow(int pin) {
  return digitalRead(pin) == LOW;
}

// Hold A LOW and read B and D, then hold C LOW and read them again.
int scanTilt() {
  releaseAll();
  holdLow(PIN_A);
  delayMicroseconds(10);        // let the lines settle
  bool ab = readsLow(PIN_B);    // A-B joined: left edge down
  bool da = readsLow(PIN_D);    // D-A joined: header edge down

  releaseAll();
  holdLow(PIN_C);
  delayMicroseconds(10);
  bool bc = readsLow(PIN_B);    // B-C joined: top edge down
  bool cd = readsLow(PIN_D);    // C-D joined: right edge down

  releaseAll();
  if (ab) return LEFT_DOWN;
  if (da) return HEADER_DOWN;
  if (bc) return TOP_DOWN;
  if (cd) return RIGHT_DOWN;
  return NO_SIDE;
}

void setup() {
  Serial.begin(115200);
  releaseAll();
}

void loop() {
  Serial.println(SIDE_NAME[scanTilt()]);
  delay(200);                   // five scans a second, to keep it readable
}
