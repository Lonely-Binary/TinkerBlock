/*
  Rotary Potentiometer - calibrating the turn            TK08 / /p/tk08

  Wiring. Count from the square pad on the TinkerBlock board, knob
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (the full turn puts VCC on your analog pin)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Serial Monitor        115200; type a key, Enter, to answer
    No library needed.
*/

// The pin SIGNAL is wired to, picked for the board you compile for.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
#if defined(ARDUINO_ARCH_AVR)
const int POT_PIN = A0;
#elif defined(ARDUINO_ARCH_RP2040)
const int POT_PIN = 26;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
const int POT_PIN = 4;
#else
const int POT_PIN = 34;         // the classic ESP32
#endif

const int MARKS = 5;
const int MARK_PCT[MARKS] = {0, 25, 50, 75, 100};
int table[MARKS];               // the reading at each mark

int readSteady() {              // sixteen readings, averaged
  long sum = 0;
  for (int i = 0; i < 16; i++) sum += analogRead(POT_PIN);
  return sum / 16;
}

void waitForEnter() {
  while (Serial.available()) Serial.read();
  while (!Serial.available()) delay(10);
  delay(50);
  while (Serial.available()) Serial.read();
}

// Position in percent: straight lines between the marks.
float positionOf(int reading) {
  if (reading <= table[0]) return MARK_PCT[0];
  for (int i = 1; i < MARKS; i++) {
    if (reading <= table[i]) {
      int span = table[i] - table[i - 1];
      float f = span > 0 ? float(reading - table[i - 1]) / span : 0;
      return MARK_PCT[i - 1] + f * (MARK_PCT[i] - MARK_PCT[i - 1]);
    }
  }
  return MARK_PCT[MARKS - 1];
}

void setup() {
  Serial.begin(115200);
#if !defined(ARDUINO_ARCH_AVR)
  analogReadResolution(12);     // the Pico's core starts at 10 bits
#endif
  delay(2000);                  // time to open the Serial Monitor
  for (int i = 0; i < MARKS; i++) {
    Serial.print("Turn the knob to ");
    Serial.print(MARK_PCT[i]);
    Serial.println(" % and press Enter.");
    waitForEnter();
    table[i] = readSteady();
    Serial.print("  reads ");
    Serial.println(table[i]);
  }
}

void loop() {
  int reading = readSteady();
  Serial.print(reading);
  Serial.print("  ->  ");
  Serial.print(positionOf(reading), 1);
  Serial.println(" %");
  delay(200);
}
