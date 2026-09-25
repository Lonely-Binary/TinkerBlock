/*
  Slim Joystick - first read                             TK21 / /p/tk21

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
// in mV with 3V3 on the header: halfway to the next level down.
const char* BUTTON_NAME[] = {"U11", "U10", "U5", "U7", "U8", "U6"};
const int LIMIT_MV[] = {2475, 1375, 963, 743, 605, 275};

// Millivolts on an analog pin, on any of the four boards.
float readMv(int pin) {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(pin);     // calibrated in the chip
#else
  return analogRead(pin) * FULL_SCALE_MV / 1023.0;
#endif
}

// Which button BTNS shows: 0 to 5 in ladder order, or -1 for none.
int whichButton(float mv) {
  for (int i = 0; i < 6; i++) {
    if (mv > LIMIT_MV[i]) return i;
  }
  return -1;
}

void setup() {
  Serial.begin(115200);
  pinMode(KEY_PIN, INPUT);        // R1 on the board pulls it down
}

void loop() {
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int key = digitalRead(KEY_PIN); // HIGH while the stick is pushed down
  float btns = readMv(BTNS_PIN);
  int b = whichButton(btns);

  Serial.print("X ");
  Serial.print(x);
  Serial.print("  Y ");
  Serial.print(y);
  Serial.print("  KEY ");
  Serial.print(key);
  Serial.print("  BTNS ");
  Serial.print(btns, 0);
  Serial.print(" mV  ");
  Serial.println(b < 0 ? "none" : BUTTON_NAME[b]);
  delay(200);                     // five lines a second
}
