/*
  Voltmeter - first reading                              TK09 / /p/tk09

  Wiring. Count from the square pad on the TinkerBlock board, terminal
  at the top, header at the bottom:

    GND    -> GND
    NC     -> nothing   (both NC pins are unconnected on the board)
    SIG    -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Terminal, same way up. Nothing is printed on it:
    left screw   GND: the minus side of what you measure
    right screw  +:   the plus side. DC only. At most 25 V on an Uno,
                      about 15.5 V on an ESP32 or ESP32-S3, 16.5 V on
                      a Pico. Never mains.

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The pin SIG is wired to. Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SIG_PIN = A0;

// Uno: 5.0 and 1023. Pico: 3.3 and 1023. The ESP32s do not use these.
const float VREF = 5.0;
const float ADC_MAX = 1023.0;

// The board's divider: 30 k over 7.5 k, so the terminal is 5 x SIG.
const float SCALE = 5.0;

float sigVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(SIG_PIN) / 1000.0;  // calibrated
#else
  return analogRead(SIG_PIN) * VREF / ADC_MAX;
#endif
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  float sig = sigVolts();
  Serial.print("SIG ");
  Serial.print(sig, 3);
  Serial.print(" V   input ");
  Serial.print(sig * SCALE, 2);
  Serial.println(" V");
  delay(500);
}
