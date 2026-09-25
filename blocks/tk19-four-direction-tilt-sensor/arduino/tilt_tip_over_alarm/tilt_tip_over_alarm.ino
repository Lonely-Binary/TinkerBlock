/*
  4-Direction Tilt Sensor - a tip-over alarm             TK19 / /p/tk19

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND  -> GND
    VCC  -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    A    -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 4 on an
            ESP32-S3, GP10 on a Raspberry Pi Pico
    B    -> D3, GPIO 26, GPIO 5, GP11      (same order)
    C    -> D4, GPIO 27, GPIO 6, GP12
    D    -> D5, GPIO 32, GPIO 7, GP13

  The block stands on edge on the box's wall, header edge down.

  A TK01 XL LED on a second pin is the alarm (optional):
    GND    -> GND
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 15 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

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
// The GPIO number the TK01's SIGNAL is wired to.
// Uno: 9. ESP32: 4. ESP32-S3: 15. Pico: 15.
const int LED_PIN = 15;

const int STABLE_READS = 5;     // 5 scans, 10 ms apart: 50 ms
const int SCAN_MS = 10;
const unsigned long TIP_MS = 1000;

const int NO_SIDE = 0;
const int LEFT_DOWN = 1;
const int TOP_DOWN = 2;
const int RIGHT_DOWN = 3;
const int HEADER_DOWN = 4;
const char* SIDE_NAME[] = {"none", "left", "top", "right", "header"};
const int UPRIGHT = HEADER_DOWN;  // standing on edge, header down

void releaseAll() {
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_C, INPUT_PULLUP);
  pinMode(PIN_D, INPUT_PULLUP);
}

void holdLow(int pin) {
  digitalWrite(pin, LOW);       // LOW first, then output
  pinMode(pin, OUTPUT);
}

bool readsLow(int pin) {
  return digitalRead(pin) == LOW;
}

int scanTilt() {
  releaseAll();
  holdLow(PIN_A);
  delayMicroseconds(10);
  bool ab = readsLow(PIN_B);    // left
  bool da = readsLow(PIN_D);    // header

  releaseAll();
  holdLow(PIN_C);
  delayMicroseconds(10);
  bool bc = readsLow(PIN_B);    // top
  bool cd = readsLow(PIN_D);    // right

  releaseAll();
  if (ab) return LEFT_DOWN;
  if (da) return HEADER_DOWN;
  if (bc) return TOP_DOWN;
  if (cd) return RIGHT_DOWN;
  return NO_SIDE;
}

int candidate = UPRIGHT;
int count = 0;
int believed = UPRIGHT;
bool tipped = false;
unsigned long tippedAt = 0;
bool alarmOn = false;

void setup() {
  Serial.begin(115200);
  releaseAll();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int side = scanTilt();
  if (side == candidate) {
    if (count < STABLE_READS) count++;
  } else {
    candidate = side;
    count = 1;
  }
  if (count >= STABLE_READS && candidate != believed) {
    believed = candidate;
    Serial.println(SIDE_NAME[believed]);
  }

  unsigned long now = millis();
  if (believed == UPRIGHT) {
    tipped = false;             // upright again: stop the clock
  } else if (!tipped) {
    tipped = true;              // just tipped: start the clock
    tippedAt = now;
  }

  if (tipped && !alarmOn && now - tippedAt >= TIP_MS) {
    alarmOn = true;             // latched: only a reset clears it
    digitalWrite(LED_PIN, HIGH);
    Serial.println("tipped over: alarm");
  }
  delay(SCAN_MS);
}
