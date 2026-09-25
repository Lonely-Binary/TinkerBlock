/*
  Piezo-Ceramic Sensor - a tap switch                   TK59 / /p/tk59

  Each tap on the disc turns the TK01 XL LED on or off, and prints
  how hard it was.

  Wiring, the TK59. Count from the square pad, parts up, header on
  the left. Two wires: the block needs no supply.

    GND    -> GND
    NC     -> nothing   (connected to nothing on the board)
    NC     -> nothing   (nor is this one: there is no VCC pin)
    SIGNAL -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
              ESP32-S3, GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way from its square pad:

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
const int PIEZO_PIN = A0;
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 15.
const int LED_PIN = 9;

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;

// A tap is a rise of this much above the lowest reading since
// the last one. Lower it for lighter taps, raise it if the LED
// switches on its own.
const int RISE_MV = 150;
// Deaf for this long after each tap, so one tap's ringing
// cannot switch the light twice.
const unsigned long LOCKOUT_MS = 150;

int floorMv = 0;          // lowest reading since the last tap
bool armed = true;
unsigned long tapAt = 0;
bool lampOn = false;

int readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(PIEZO_PIN);   // calibrated in the chip
#else
  return analogRead(PIEZO_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Tap the disc.");
}

void loop() {
  int mv = readMilliVolts();

  // Follow the level down as the last hump drains away, so a
  // second tap on its tail is measured from where it had got to.
  if (mv < floorMv) floorMv = mv;

  if (armed && mv - floorMv >= RISE_MV) {
    // Follow the rise to its top, a few milliseconds at most.
    int peak = mv;
    unsigned long start = millis();
    while (millis() - start < 5) {
      int v = readMilliVolts();
      if (v > peak) peak = v;
    }

    lampOn = !lampOn;
    digitalWrite(LED_PIN, lampOn ? HIGH : LOW);
    Serial.print("tap, peak ");
    Serial.print(peak);
    Serial.println(" mV");

    armed = false;
    tapAt = millis();
    floorMv = peak;
  }

  if (!armed && millis() - tapAt >= LOCKOUT_MS) armed = true;
}
