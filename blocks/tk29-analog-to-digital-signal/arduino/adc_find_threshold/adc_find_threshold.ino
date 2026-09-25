/*
  Analog to Digital Signal - find the threshold          TK29 / /p/tk29

  A TK08 Rotary Potentiometer, or any analog block, pushed into IN,
  parts facing the same way. Either switch position.

  Wiring, OUT to your board. Count from the square pad, parts up,
  OUT at the bottom:

    GND -> GND
    VCC -> 5V on an Uno; 3V3 on an ESP32, ESP32-S3 or Pico
           (DIG is pulled up to VCC)
    DIG -> D2 on an Uno, GPIO 25 on an ESP32, GPIO 7 on an
           ESP32-S3, GP15 on a Raspberry Pi Pico
    ANA -> A0 on an Uno, GPIO 34 on an ESP32, GPIO 4 on an
           ESP32-S3, GP26 on a Raspberry Pi Pico

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

// Uno and Pico only: the ADC's full scale, in mV.
// Uno: 5000. Pico: 3300.
const float FULL_SCALE_MV = 5000.0;

int lastDig;

float readMilliVolts() {
#if defined(ARDUINO_ARCH_ESP32)
  return analogReadMilliVolts(ANA_PIN);   // calibrated in the chip
#else
  return analogRead(ANA_PIN) * FULL_SCALE_MV / 1023.0;
#endif
}

void setup() {
  Serial.begin(115200);
  pinMode(DIG_PIN, INPUT);        // the block has its own pull-up
  lastDig = digitalRead(DIG_PIN);
  Serial.println("Sweep the input block slowly through the knob.");
}

void loop() {
  int dig = digitalRead(DIG_PIN);
  if (dig != lastDig) {           // DIG has just changed...
    float mv = readMilliVolts();  // ...so ANA is at the threshold
    lastDig = dig;
    Serial.print(dig == HIGH ? "HIGH at " : "LOW at ");
    Serial.print(mv, 0);
    Serial.println(" mV");
  }
}
