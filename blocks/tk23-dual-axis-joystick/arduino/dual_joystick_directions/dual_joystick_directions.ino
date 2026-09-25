/*
  Dual Axis Joystick - eight directions                  TK23 / /p/tk23

  Wiring. Parts up, header along the bottom. Count from the square pad,
  which is GND at the left-hand end, rightwards:

    GND -> GND
    VCC -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC  -> nothing   (in no net on the board)
    X   -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
           ESP32-S3, GP26 on a Raspberry Pi Pico      (analog)
    Y   -> A1, GPIO 35, GPIO 5, GP27      (same order, analog)
    SW  -> D2, GPIO 25, GPIO 7, GP15      (digital)

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pins X, Y and SW are wired to.
// Uno: A0 A1 2. ESP32: 34 35 25. ESP32-S3: 4 5 7. Pico: 26 27 15.
const int X_PIN = 4;
const int Y_PIN = 5;
const int SW_PIN = 7;

#if defined(ARDUINO_ARCH_ESP32)
const float ADC_MAX = 4095;
#else
const float ADC_MAX = 1023;
#endif

const int DEAD_ZONE = 10;       // as a distance from the centre
const int EDGE_GUESS = 30;      // per cent of full scale, until learned
const unsigned long DEBOUNCE_MS = 20;

// From the first read: SWAP_XY true if X moved when you pushed towards
// and away from the header; a sign -1 if that direction came out wrong.
const bool SWAP_XY = false;
const int X_SIGN = 1;
const int Y_SIGN = 1;

const char* NAME[] = {"right", "up-right", "up", "up-left",
                      "left", "down-left", "down", "down-right"};

float centreX, loX, hiX, centreY, loY, hiY;
int lastDir = -2;               // -1 is centre; -2 is not yet known
int lastSw = LOW;
unsigned long quietUntil = 0;

float average(int pin) {
  float sum = 0;
  for (int i = 0; i < 16; i++) sum += analogRead(pin);
  return sum / 16;
}

int axis(float v, float centre, float &lo, float &hi) {
  if (v < lo) lo = v;
  if (v > hi) hi = v;
  float p;
  if (v >= centre) p = (v - centre) * 100.0 / (hi - centre);
  else p = (v - centre) * 100.0 / (centre - lo);
  return constrain((int)p, -100, 100);
}

// 0..7 for the eight slices, counting round from right; -1 for centre.
int direction(int x, int y) {
  if (x * x + y * y < DEAD_ZONE * DEAD_ZONE) return -1;
  float a = atan2((float)y, (float)x);    // -pi..pi
  int i = (int)round(a / (PI / 4));
  return (i + 8) % 8;
}

void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT);         // R10 on the board pulls it down
  delay(200);                     // hands off the stick
  float guess = ADC_MAX * EDGE_GUESS / 100.0;
  centreX = average(X_PIN);
  centreY = average(Y_PIN);
  loX = centreX - guess;
  hiX = centreX + guess;
  loY = centreY - guess;
  hiY = centreY + guess;
}

void loop() {
  int a = axis(average(X_PIN), centreX, loX, hiX);
  int b = axis(average(Y_PIN), centreY, loY, hiY);
  int x = X_SIGN * (SWAP_XY ? b : a);
  int y = Y_SIGN * (SWAP_XY ? a : b);

  int d = direction(x, y);
  if (d != lastDir) {
    Serial.println(d < 0 ? "centre" : NAME[d]);
    lastDir = d;
  }

  unsigned long now = millis();
  if (now >= quietUntil) {
    int sw = digitalRead(SW_PIN);
    if (sw != lastSw) {
      if (sw == HIGH) Serial.println("click");
      lastSw = sw;
      quietUntil = now + DEBOUNCE_MS;
    }
  }
  delay(10);
}
