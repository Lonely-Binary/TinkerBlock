/*
  Reflective Optical Sensor - first reading             TK57 / /p/tk57

  Wiring. Count from the square pad on the TinkerBlock board, parts
  up, header at the bottom:

    GND    -> GND
    VCC    -> 3V3 on an ESP32, ESP32-S3 or Pico; 5V on an Uno.
              SIGNAL rises to VCC over a dark surface, so VCC
              is the voltage your board's pins run at.
    NC     -> nothing   (unconnected on the board)
    SIGNAL -> GPIO 4 on an ESP32-S3, GPIO 34 on an ESP32,
              A0 on an Uno, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. ESP32S3 Dev Module
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int SENSOR_PIN = 4;

#if defined(ARDUINO_ARCH_ESP32)
const int FULL = 4095;   // 12-bit on the ESP32 family
#else
const int FULL = 1023;   // 10-bit on an Uno and a Pico
#endif

void setup() {
  Serial.begin(115200);
}

void loop() {
  // More light back, LOWER: the sensor pulls SIGNAL down.
  int level = analogRead(SENSOR_PIN);

  // A bar as long as the reading: long over black or air,
  // short over white paper.
  int bars = map(level, 0, FULL, 0, 40);
  Serial.print(level);
  Serial.print('\t');
  for (int i = 0; i < bars; i++) Serial.print('#');
  Serial.println();

  delay(100);
}
