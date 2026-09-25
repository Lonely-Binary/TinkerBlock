/*
  Rotary Potentiometer - first read                      TK08 / /p/tk08

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
    No library needed.
*/

// The pin SIGNAL is wired to, picked for the board you compile for.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
#if defined(ARDUINO_ARCH_AVR)
const int POT_PIN = A0;
const int ADC_MAX = 1023;       // 10 bits
const float VREF = 5.0;         // measured against 5 V
#elif defined(ARDUINO_ARCH_RP2040)
const int POT_PIN = 26;
const int ADC_MAX = 4095;       // 12 bits, set in setup()
const float VREF = 3.3;
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
const int POT_PIN = 4;
const int ADC_MAX = 4095;
const float VREF = 3.3;
#else
const int POT_PIN = 34;         // the classic ESP32
const int ADC_MAX = 4095;
const float VREF = 3.3;
#endif

void setup() {
  Serial.begin(115200);
#if !defined(ARDUINO_ARCH_AVR)
  analogReadResolution(12);     // the Pico's core starts at 10 bits
#endif
}

void loop() {
  int reading = analogRead(POT_PIN);
  float volts = reading * VREF / ADC_MAX;

  Serial.print(reading);
  Serial.print("   ");
  Serial.print(volts, 2);
  Serial.println(" V");
  delay(200);                   // five lines a second
}
