/*
  Analog Microphone - resting level and swing           TK27 / /p/tk27

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int MIC_PIN = A0;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;
// Several cycles of the lowest sound the block passes.
const unsigned long WINDOW_MS = 50;
const unsigned long CALIBRATE_MS = 1000;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(MIC_PIN);   // calibrated in the chip
#else
  return analogRead(MIC_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Measuring the resting level. Keep quiet...");

  float sum = 0;
  long n = 0;
  unsigned long start = millis();
  while (millis() - start < CALIBRATE_MS) {
    sum += readMilliVolts();
    n++;
  }
  Serial.print("Resting level ");
  Serial.print(sum / n, 0);
  Serial.println(" mV");
}

void loop() {
  // The highest and lowest reading in one window.
  float lo = 100000;
  float hi = -1;
  unsigned long start = millis();
  while (millis() - start < WINDOW_MS) {
    float mv = readMilliVolts();
    if (mv < lo) lo = mv;
    if (mv > hi) hi = mv;
  }

  Serial.print("swing ");
  Serial.print(hi - lo, 0);   // peak to peak: louder, bigger
  Serial.println(" mV");
}
