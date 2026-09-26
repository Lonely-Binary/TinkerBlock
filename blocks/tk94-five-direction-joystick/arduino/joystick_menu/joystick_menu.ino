/*
  Five-Direction Joystick - a menu                     TK94 / /p/tk94

  Up and down move through three settings, left and right change
  the one you are on, and a press sets it back to 0. Each push
  counts once, however long you hold it.

  Wiring. Parts up, header at the bottom. Count from the square pad,
  which is GND at the left-hand end:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC     -> nothing: it is connected to nothing on the board
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico       (analog)

  The names are for the block held with the header on the left,
  the way the words on its front read.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SIGNAL_PIN = 4;

const char* NAME[] = {"right", "down", "left", "press", "up"};
const int LIMIT[] = {722, 359, 227, 146, 56};
enum { RIGHT, DOWN, LEFT, PRESS, UP };

// A push is believed once it has read the same for SETTLE_MS.
// Letting go takes about 3 ms to fall through the lower levels.
const unsigned long SETTLE_MS = 10;
const unsigned long READ_MS = 5;

const char* ITEM[] = {"Speed", "Level", "Volume"};
const int ITEMS = 3;
const int MAX_VALUE = 9;
int value[ITEMS] = {0, 0, 0};
int cursor = 0;

int candidate = -1;             // what the readings say now
unsigned long since = 0;        // since when
int believed = -1;              // the push the sketch acted on

int readPermille() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(SIGNAL_PIN) * 1000L / 3300;
#else
  return analogRead(SIGNAL_PIN) * 1000L / 1023;
#endif
}

int whichPush(int permille) {
  for (int i = 0; i < 5; i++) {
    if (permille > LIMIT[i]) return i;
  }
  return -1;
}

void show() {
  Serial.print("> ");
  Serial.print(ITEM[cursor]);
  Serial.print(" ");
  Serial.println(value[cursor]);
}

// One push, acted on once.
void act(int d) {
  if (d == UP) cursor = (cursor + ITEMS - 1) % ITEMS;
  if (d == DOWN) cursor = (cursor + 1) % ITEMS;
  if (d == LEFT && value[cursor] > 0) value[cursor]--;
  if (d == RIGHT && value[cursor] < MAX_VALUE) value[cursor]++;
  if (d == PRESS) value[cursor] = 0;
  show();
}

void setup() {
  Serial.begin(115200);
  show();
}

void loop() {
  int d = whichPush(readPermille());
  unsigned long now = millis();
  if (d != candidate) {           // changed: start timing it
    candidate = d;
    since = now;
  } else if (now - since >= SETTLE_MS && d != believed) {
    believed = d;                 // steady, and new
    if (d >= 0) act(d);           // letting go is not a push
  }
  delay(READ_MS);
}
