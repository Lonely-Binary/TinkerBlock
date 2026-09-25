/*
  Linear Hall Effect Sensor - first reading             TK70 / /p/tk70

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              Your board's logic voltage: a strong field takes
              SIGNAL up to about 0.8 x VCC.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Keep magnets away while it starts: it measures its zero then.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int HALL_PIN = 4;

#if defined(ARDUINO_ARCH_AVR)
// Uno, VCC on 5V: 3.25 mV per gauss, typical; ADC full scale 5 V.
const float MV_PER_GAUSS = 3.25;
const float FULL_SCALE_MV = 5000.0;
#else
// VCC on 3V3: about 2.1 mV per gauss; Pico's ADC full scale 3.3 V.
const float MV_PER_GAUSS = 2.1;
const float FULL_SCALE_MV = 3300.0;
#endif

// Which pole, held to the front, raises the output. The data
// sheet's SOT-23 drawing says north. If a compass says otherwise,
// make this false.
const bool NORTH_RAISES = true;

float zeroMv;   // SIGNAL with no magnet near: about half of VCC

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(HALL_PIN);   // calibrated in the chip
#else
  return analogRead(HALL_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

// The average of n readings: steadier than one.
float averageMv(int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) sum += readMilliVolts();
  return sum / n;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  zeroMv = averageMv(64);   // no magnet near, please
  Serial.print("zero: ");
  Serial.print(zeroMv, 0);
  Serial.println(" mV");
}

void loop() {
  float gauss = (averageMv(16) - zeroMv) / MV_PER_GAUSS;

  Serial.print(gauss, 0);
  Serial.print(" G");
  if (fabs(gauss) >= 5) {
    bool raised = gauss > 0;
    Serial.print(raised == NORTH_RAISES ? "  north" : "  south");
  }
  if (fabs(gauss) > 460) {
    Serial.print("  (past the linear range)");
  }
  Serial.println();
  delay(250);
}
