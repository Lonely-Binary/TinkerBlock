/*
  Slim Joystick - a centre and a dead zone               TK21 / /p/tk21

  Wiring. Parts up, header along the top. Count from the square pad,
  which is GND at the right-hand end, leftwards:

    GND  -> GND
    3V3  -> 3V3 on every board, the Uno included (it is printed 3V3)
    BTNS -> A2 on an Uno, GPIO 32 on an ESP32, GPIO 6 on an
            ESP32-S3, GP28 on a Raspberry Pi Pico     (analog)
    X    -> A0, GPIO 34, GPIO 4, GP26      (same order, analog)
    Y    -> A1, GPIO 35, GPIO 5, GP27      (analog)
    KEY  -> D2, GPIO 25, GPIO 7, GP15      (digital)

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pins X, Y, BTNS and KEY are wired to.
// Uno: A0 A1 A2 2. ESP32: 34 35 32 25. ESP32-S3: 4 5 6 7. Pico: 26 27 28 15.
const int X_PIN = 4;
const int Y_PIN = 5;
const int BTNS_PIN = 6;
const int KEY_PIN = 7;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;

const int DEAD_ZONE = 10;          // per cent either side of the centre
const float EDGE_GUESS_MV = 1000;  // the ends, until the stick shows them

float centreX, loX, hiX;           // measured, and the furthest seen
float centreY, loY, hiY;

float readMv(int pin) {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(pin);     // calibrated in the chip
#else
  return analogRead(pin) * FULL_SCALE_MV / 1023.0;
#endif
}

// The mean of 16 readings: steadier than one.
float averageMv(int pin) {
  float sum = 0;
  for (int i = 0; i < 16; i++) sum += readMv(pin);
  return sum / 16;
}

// -100..100 from the centre, widening the ends as the stick finds them.
int axis(float mv, float centre, float &lo, float &hi) {
  if (mv < lo) lo = mv;
  if (mv > hi) hi = mv;
  float p;
  if (mv >= centre) p = (mv - centre) * 100.0 / (hi - centre);
  else p = (mv - centre) * 100.0 / (centre - lo);
  int out = constrain((int)p, -100, 100);
  if (abs(out) < DEAD_ZONE) out = 0;   // the dead zone
  return out;
}

void setup() {
  Serial.begin(115200);
  pinMode(KEY_PIN, INPUT);        // R1 on the board pulls it down
  delay(200);                     // hands off the stick
  centreX = averageMv(X_PIN);
  centreY = averageMv(Y_PIN);
  loX = centreX - EDGE_GUESS_MV;
  hiX = centreX + EDGE_GUESS_MV;
  loY = centreY - EDGE_GUESS_MV;
  hiY = centreY + EDGE_GUESS_MV;
}

void loop() {
  int x = axis(averageMv(X_PIN), centreX, loX, hiX);
  int y = axis(averageMv(Y_PIN), centreY, loY, hiY);

  Serial.print("X ");
  Serial.print(x);
  Serial.print("  Y ");
  Serial.println(y);
  delay(100);
}
