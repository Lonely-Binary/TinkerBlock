/*
  Dual Axis Joystick - a centre and a dead zone          TK23 / /p/tk23

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

// The ADC's top count.
#if defined(ARDUINO_ARCH_ESP32)
const float ADC_MAX = 4095;
#else
const float ADC_MAX = 1023;
#endif

const int DEAD_ZONE = 10;       // per cent either side of the centre
const int EDGE_GUESS = 30;      // per cent of full scale, until learned

float centreX, loX, hiX;        // measured, and the furthest seen
float centreY, loY, hiY;

// The mean of 16 readings: steadier than one.
float average(int pin) {
  float sum = 0;
  for (int i = 0; i < 16; i++) sum += analogRead(pin);
  return sum / 16;
}

// -100..100 from the centre, widening the ends as the stick finds them.
int axis(float v, float centre, float &lo, float &hi) {
  if (v < lo) lo = v;
  if (v > hi) hi = v;
  float p;
  if (v >= centre) p = (v - centre) * 100.0 / (hi - centre);
  else p = (v - centre) * 100.0 / (centre - lo);
  return constrain((int)p, -100, 100);
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
  int x = axis(average(X_PIN), centreX, loX, hiX);
  int y = axis(average(Y_PIN), centreY, loY, hiY);
  if (abs(x) < DEAD_ZONE) x = 0;  // the dead zone
  if (abs(y) < DEAD_ZONE) y = 0;

  Serial.print("X ");
  Serial.print(x);
  Serial.print("  Y ");
  Serial.print(y);
  Serial.print("  SW ");
  Serial.println(digitalRead(SW_PIN));
  delay(100);
}
