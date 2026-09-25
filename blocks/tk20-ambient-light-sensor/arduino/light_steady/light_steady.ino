/*
  Ambient Light Sensor - a steady reading                TK20 / /p/tk20

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
const int LIGHT_PIN = A0;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;

// 5 cycles of a 100 Hz ripple, 6 of a 120 Hz one.
const unsigned long WINDOW_MS = 50;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(LIGHT_PIN);   // calibrated in the chip
#else
  return analogRead(LIGHT_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

// The mean of everything read in WINDOW_MS.
float readSteadyMilliVolts() {
  float sum = 0;
  long n = 0;
  unsigned long start = millis();
  while (millis() - start < WINDOW_MS) {
    sum += readMilliVolts();
    n++;
  }
  return sum / n;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("one read ");
  Serial.print(readMilliVolts(), 0);
  Serial.print(" mV    50 ms mean ");
  Serial.print(readSteadyMilliVolts(), 0);
  Serial.println(" mV");
  delay(250);
}
