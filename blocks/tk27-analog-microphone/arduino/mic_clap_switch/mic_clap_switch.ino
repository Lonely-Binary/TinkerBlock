/*
  Analog Microphone - a clap switch                     TK27 / /p/tk27

  Wiring, the TK27. Count from the square pad, parts up, header at
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

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int MIC_PIN = A0;
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;
const unsigned long WINDOW_MS = 50;
const unsigned long CALIBRATE_MS = 1000;
const float CLAP_MARGIN_MV = 200;       // how much louder than quiet
const unsigned long LOCKOUT_MS = 300;   // deaf after each switch

float thresholdMv;
bool lampOn = false;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(MIC_PIN);   // calibrated in the chip
#else
  return analogRead(MIC_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

// Highest minus lowest over one window, in mV.
float swingMilliVolts() {
  float lo = 100000;
  float hi = -1;
  unsigned long start = millis();
  while (millis() - start < WINDOW_MS) {
    float mv = readMilliVolts();
    if (mv < lo) lo = mv;
    if (mv > hi) hi = mv;
  }
  return hi - lo;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);

  Serial.println("Measuring the quiet swing. Keep quiet...");
  float quiet = 0;
  unsigned long start = millis();
  while (millis() - start < CALIBRATE_MS) {
    float s = swingMilliVolts();
    if (s > quiet) quiet = s;
  }
  thresholdMv = quiet + CLAP_MARGIN_MV;
  Serial.print("Clap above ");
  Serial.print(thresholdMv, 0);
  Serial.println(" mV");
}

void loop() {
  float swing = swingMilliVolts();
  if (swing > thresholdMv) {
    lampOn = !lampOn;
    digitalWrite(LED_PIN, lampOn ? HIGH : LOW);
    Serial.print(swing, 0);
    Serial.println(lampOn ? " mV  on" : " mV  off");
    delay(LOCKOUT_MS);   // let the clap and its echo die away
  }
}
