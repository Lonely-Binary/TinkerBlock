/*
  NTC Thermistor - counts to degrees                     TK12 / /p/tk12

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
              (SIGNAL is a fraction of VCC, so it stays in range)
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

#include <math.h>

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SENSOR_PIN = A0;

const float R_FIXED = 10000.0;   // the 10 kOhm from VCC to SIGNAL
const float R25 = 10000.0;       // the thermistor at 25 C: the "103"
const float B = 3950.0;          // its B value: the "3950"
const float T0 = 298.15;         // 25 C in kelvin
const float VCC_MV = 3300.0;     // ESP32 only: your 3V3 pin, measured

// The thermistor's resistance, or NAN if SIGNAL is at either end.
float thermistorOhms() {
#if defined(ARDUINO_ARCH_ESP32)
  // Own reference: calibrated millivolts, over what VCC is.
  float mv = analogReadMilliVolts(SENSOR_PIN);
  if (mv <= 0 || mv >= VCC_MV) return NAN;
  return R_FIXED * mv / (VCC_MV - mv);
#else
  // Uno, Pico: the count is already a fraction of VCC.
  float n = analogRead(SENSOR_PIN);
  if (n <= 0 || n >= 1023) return NAN;
  return R_FIXED * n / (1023.0 - n);
#endif
}

// The B equation, backwards: resistance to degrees Celsius.
float celsius(float ohms) {
  float invT = 1.0 / T0 + log(ohms / R25) / B;
  return 1.0 / invT - 273.15;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  float r = thermistorOhms();

  if (isnan(r)) {
    Serial.println("SIGNAL at 0: check VCC. At the top: check GND.");
  } else {
    Serial.print(r, 0);
    Serial.print(" ohm   ");
    Serial.print(celsius(r), 1);
    Serial.println(" C");
  }
  delay(1000);
}
