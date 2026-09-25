/*
  Rotary Potentiometer - smoothing                       TK08 / /p/tk08

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
    Tools > Serial Plotter        at 115200, Serial Monitor closed
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

const int SMOOTH_N = 8;         // each reading moves the average 1/8
float average;

void setup() {
  Serial.begin(115200);
#if !defined(ARDUINO_ARCH_AVR)
  analogReadResolution(12);     // the Pico's core starts at 10 bits
#endif
  average = analogRead(POT_PIN);    // start from a real reading
}

void loop() {
  int raw = analogRead(POT_PIN);
  average += (raw - average) / SMOOTH_N;

  Serial.print("raw:");
  Serial.print(raw);
  Serial.print(" smooth:");
  Serial.println(average, 0);
  delay(10);                    // a hundred readings a second
}
