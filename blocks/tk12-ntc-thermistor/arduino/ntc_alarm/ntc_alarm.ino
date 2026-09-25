/*
  NTC Thermistor - a temperature alarm                   TK12 / /p/tk12

  Wiring, the TK12. Count from the square pad, parts up, header at
  the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP15 on a Raspberry Pi Pico

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
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

const float ALARM_C = 28.0;      // on at or above this
const float HYST_C = 1.0;        // off only below ALARM_C - HYST_C

const int SAMPLES = 32;
const float R_FIXED = 10000.0;   // the 10 kOhm from VCC to SIGNAL
const float R25 = 10000.0;       // the thermistor at 25 C: the "103"
const float B = 3950.0;          // its B value: the "3950"
const float T0 = 298.15;         // 25 C in kelvin
const float VCC_MV = 3300.0;     // ESP32 only: your 3V3 pin, measured

bool alarmOn = false;

float thermistorOhms() {
  float sum = 0;
#if defined(ARDUINO_ARCH_ESP32)
  for (int i = 0; i < SAMPLES; i++) sum += analogReadMilliVolts(SENSOR_PIN);
  float mv = sum / SAMPLES;
  if (mv <= 0 || mv >= VCC_MV) return NAN;
  return R_FIXED * mv / (VCC_MV - mv);
#else
  for (int i = 0; i < SAMPLES; i++) sum += analogRead(SENSOR_PIN);
  float n = sum / SAMPLES;
  if (n <= 0 || n >= 1023) return NAN;
  return R_FIXED * n / (1023.0 - n);
#endif
}

float celsius(float ohms) {
  float invT = 1.0 / T0 + log(ohms / R25) / B;
  return 1.0 / invT - 273.15;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  float r = thermistorOhms();
  if (isnan(r)) {
    Serial.println("SIGNAL at 0: check VCC. At the top: check GND.");
    delay(500);
    return;
  }
  float c = celsius(r);

  if (!alarmOn && c >= ALARM_C) alarmOn = true;
  if (alarmOn && c < ALARM_C - HYST_C) alarmOn = false;
  digitalWrite(LED_PIN, alarmOn ? HIGH : LOW);

  Serial.print(c, 2);
  Serial.println(alarmOn ? " C  ALARM" : " C");
  delay(250);
}
