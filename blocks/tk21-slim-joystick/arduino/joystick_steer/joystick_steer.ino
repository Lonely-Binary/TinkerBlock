/*
  Slim Joystick - steering a dot                         TK21 / /p/tk21

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

// The buttons in ladder order, and the line under each one's level,
// in mV with 3V3 on the header.
const int LIMIT_MV[] = {2475, 1375, 963, 743, 605, 275};
const int U11 = 0, U10 = 1, U5 = 2, U7 = 3, U8 = 4, U6 = 5;

const int DEAD_ZONE = 10;          // per cent either side of the centre
const float EDGE_GUESS_MV = 1000;  // the ends, until the stick shows them
const int FIELD_W = 21;
const int FIELD_H = 9;
const int STEP_PER = 50;           // per cent of tilt per cell a frame
const int FRAME_MS = 100;
const bool FLIP_X = false;         // true if the dot goes the wrong way
const bool FLIP_Y = false;

float centreX, loX, hiX;
float centreY, loY, hiY;
int dotX = FIELD_W / 2;
int dotY = FIELD_H / 2;
bool mark[FIELD_H][FIELD_W];
int lastButton = -1;
int lastKey = LOW;

float readMv(int pin) {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(pin);     // calibrated in the chip
#else
  return analogRead(pin) * FULL_SCALE_MV / 1023.0;
#endif
}

float averageMv(int pin) {
  float sum = 0;
  for (int i = 0; i < 16; i++) sum += readMv(pin);
  return sum / 16;
}

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

// 0 to 5 in ladder order, or -1 for none. One button at a time.
int whichButton() {
  float mv = readMv(BTNS_PIN);
  for (int i = 0; i < 6; i++) {
    if (mv > LIMIT_MV[i]) return i;
  }
  return -1;
}

void press(int b) {
  if (b == U11) {                  // back to the middle
    dotX = FIELD_W / 2;
    dotY = FIELD_H / 2;
  }
  if (b == U10) memset(mark, 0, sizeof(mark));   // clear the marks
  if (b == U5) dotY = min(dotY + 1, FIELD_H - 1);  // down
  if (b == U7) dotX = max(dotX - 1, 0);            // left
  if (b == U8) dotY = max(dotY - 1, 0);            // up
  if (b == U6) dotX = min(dotX + 1, FIELD_W - 1);  // right
}

void draw() {
  Serial.println();
  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      if (x == dotX && y == dotY) Serial.print('@');
      else if (mark[y][x]) Serial.print('+');
      else Serial.print('.');
    }
    Serial.println();
  }
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
  int sx = axis(averageMv(X_PIN), centreX, loX, hiX);
  int sy = axis(averageMv(Y_PIN), centreY, loY, hiY);
  if (FLIP_X) sx = -sx;
  if (FLIP_Y) sy = -sy;
  dotX = constrain(dotX + sx / STEP_PER, 0, FIELD_W - 1);
  dotY = constrain(dotY - sy / STEP_PER, 0, FIELD_H - 1);

  int b = whichButton();
  if (b != lastButton && b >= 0) press(b);   // once per press
  lastButton = b;

  int key = digitalRead(KEY_PIN);            // HIGH while clicked
  if (key == HIGH && lastKey == LOW) mark[dotY][dotX] = true;
  lastKey = key;

  draw();
  delay(FRAME_MS);
}
