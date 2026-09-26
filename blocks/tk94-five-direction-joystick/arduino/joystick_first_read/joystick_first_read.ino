/*
  Five-Direction Joystick - first read                 TK94 / /p/tk94

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

// The pushes, highest level first, and the line under each one:
// halfway to the next level down, in thousandths of VCC.
const char* NAME[] = {"right", "down", "left", "press", "up"};
const int LIMIT[] = {722, 359, 227, 146, 56};

// SIGNAL in thousandths of VCC, on any of the four boards.
int readPermille() {
#if defined(ARDUINO_ARCH_ESP32)
  // Measured against a reference in the chip: calibrated
  // millivolts, over VCC's 3300.
  return analogReadMilliVolts(SIGNAL_PIN) * 1000L / 3300;
#else
  // The Uno and Pico measure against their supply, which is VCC.
  return analogRead(SIGNAL_PIN) * 1000L / 1023;
#endif
}

// Which push a reading is: 0 to 4 as in NAME, or -1 for none.
int whichPush(int permille) {
  for (int i = 0; i < 5; i++) {
    if (permille > LIMIT[i]) return i;
  }
  return -1;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int p = readPermille();
  int d = whichPush(p);
  Serial.print("SIGNAL ");
  Serial.print(p);
  Serial.print("/1000  ");
  Serial.println(d < 0 ? "none" : NAME[d]);
  delay(200);                     // five lines a second
}
