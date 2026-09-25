/*
  Piezo-Ceramic Sensor - first reading                  TK59 / /p/tk59

  Two wires: the block needs no supply. Count from the square pad on
  the TinkerBlock board, parts up, header on the left:

    GND    -> GND
    NC     -> nothing   (connected to nothing on the board)
    NC     -> nothing   (nor is this one: there is no VCC pin)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    Tools > Serial Plotter        at 115200, to see each tap
    No library needed.
*/

// The analog pin SIGNAL is wired to.
// Uno: A0. ESP32: 34. ESP32-S3: 4. Pico: 26.
const int PIEZO_PIN = A0;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;

// A tap is a hump: up in about a millisecond, down over a
// tenth of a second or so. The highest reading in each 20 ms
// catches the top of it.
const unsigned long WINDOW_MS = 20;

int readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(PIEZO_PIN);   // calibrated in the chip
#else
  return analogRead(PIEZO_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int peak = 0;
  unsigned long start = millis();
  while (millis() - start < WINDOW_MS) {
    int mv = readMilliVolts();
    if (mv > peak) peak = mv;
  }
  Serial.print("mV:");
  Serial.println(peak);
}
