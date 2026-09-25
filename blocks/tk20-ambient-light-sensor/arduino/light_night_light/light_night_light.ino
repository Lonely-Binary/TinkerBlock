/*
  Ambient Light Sensor - a night light                   TK20 / /p/tk20

  Wiring, the TK20. Count from the square pad, parts up, header at
  the bottom:

    GND    -> GND
    VCC    -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way. Point it away from the TK20:

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

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int LIGHT_PIN = A0;
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;
// 5 cycles of a 100 Hz ripple, 6 of a 120 Hz one.
const unsigned long WINDOW_MS = 50;

const float ON_FRACTION = 0.4;    // on below 40 % of dark-to-lit
const float OFF_FRACTION = 0.6;   // off only above 60 %

float onBelowMv;
float offAboveMv;
bool lampOn = false;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(LIGHT_PIN);   // calibrated in the chip
#else
  return analogRead(LIGHT_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

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
  pinMode(LED_PIN, OUTPUT);
  delay(1000);

  Serial.println("Calibrating. Leave the room lit...");
  delay(3000);
  float lit = readSteadyMilliVolts();
  Serial.println("Now cover the sensor with a finger...");
  delay(4000);
  float dark = readSteadyMilliVolts();
  Serial.println("Done. Uncover it.");

  if (lit - dark < 20) {
    Serial.println("Lit and dark are too close: press reset.");
  }
  onBelowMv = dark + ON_FRACTION * (lit - dark);
  offAboveMv = dark + OFF_FRACTION * (lit - dark);

  Serial.print("On below ");
  Serial.print(onBelowMv, 0);
  Serial.print(" mV, off above ");
  Serial.print(offAboveMv, 0);
  Serial.println(" mV");
}

void loop() {
  float mv = readSteadyMilliVolts();

  if (!lampOn && mv < onBelowMv) lampOn = true;
  if (lampOn && mv > offAboveMv) lampOn = false;
  digitalWrite(LED_PIN, lampOn ? HIGH : LOW);

  Serial.print(mv, 0);
  Serial.println(lampOn ? " mV  on" : " mV");
  delay(200);
}
