/*
  Analog to Digital Signal - a light switch              TK29 / /p/tk29

  A TK20 Ambient Light Sensor pushed into IN, parts facing the same
  way. Slide switch at L: DIG is HIGH while it is darker than the knob.

  Wiring, the TK29's OUT. Count from the square pad, parts up, OUT at
  the bottom:

    GND -> GND
    VCC -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
           (DIG is pulled up to VCC)
    DIG -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 7 on an
           ESP32-S3, GP15 on a Raspberry Pi Pico
    ANA -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
           ESP32-S3, GP26 on a Raspberry Pi Pico

  The TK01 XL LED, counted the same way. Point it away from the TK20:

    GND    -> GND
    NC     -> nothing   (both of its NC pins)
    SIGNAL -> D9 on an Uno, GPIO 4 on an ESP32, GPIO 5 on an
              ESP32-S3, GP14 on a Raspberry Pi Pico

  Arduino IDE
    Tools > Board                 your board, e.g. Arduino Uno
    Tools > Port                  the one that appears when you plug in
    Tools > USB CDC On Boot       Enabled   (ESP32-S3 only)
    No library needed.
*/

// DIG_PIN, then ANA_PIN.
// Uno: 2 and A0. ESP32: 25 and 34. ESP32-S3: 7 and 4. Pico: 15 and 26.
const int DIG_PIN = 2;
const int ANA_PIN = A0;
// The TK01's SIGNAL. Uno: 9. ESP32: 4. ESP32-S3: 5. Pico: 14.
const int LAMP_PIN = 9;

void setup() {
  Serial.begin(115200);
  pinMode(DIG_PIN, INPUT);          // the block has its own pull-up
  pinMode(LAMP_PIN, OUTPUT);
}

void loop() {
  int dark = digitalRead(DIG_PIN);  // at L: HIGH when darker
  digitalWrite(LAMP_PIN, dark);     // the lamp copies DIG

  Serial.print(dark == HIGH ? "dark   ANA " : "light  ANA ");
  Serial.println(analogRead(ANA_PIN));
  delay(100);
}
